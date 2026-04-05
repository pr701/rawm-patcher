/*
 * patcher.c - pattern scanner, file I/O, auto-detect
 */

#include "patcher.h"
#include <shlobj.h>
#include <knownfolders.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")

/* Pattern definitions */

#define W 0xFFFF  /* wildcard byte */

/*  3x: mov ecx,imm32 / call op_new / mov [rbx+off],rax */
static const WORD PAT_ALLOC[] = {
    0xB9, W, W, W, W,
    0xE8, W, W, W, W,
    0x48, 0x89, 0x83, 0x50, 0x03, 0x00, 0x00,
    0xB9, W, W, W, W,
    0xE8, W, W, W, W,
    0x48, 0x89, 0x83, 0xB0, 0x03, 0x00, 0x00,
    0xB9, W, W, W, W,
    0xE8, W, W, W, W
};

/*  lea eax,[rdi+r14] / cmp eax,imm32 / jg short / mov rcx,rsi */
static const WORD PAT_CHK13[] = {
    0x42, 0x8D, 0x04, 0x37,
    0x3D, W, W, W, W,
    0x7F, W,
    0x48, 0x8B, 0xCE
};

/*  lea eax,[rdi+r15] / cmp eax,imm32 / jg near / cmp byte ptr [r14],6 */
static const WORD PAT_CHK2[] = {
    0x42, 0x8D, 0x04, 0x3F,
    0x3D, W, W, W, W,
    0x0F, 0x8F, W, W, W, W,
    0x41, 0x80, 0x3E, 0x06
};

/*  mov rcx,r9 / mov r8d,imm32 / xor edx,edx / call memset / mov rcx,rsi */
static const WORD PAT_MSET[] = {
    0x49, 0x8B, 0xC9,
    0x41, 0xB8, W, W, W, W,
    0x33, 0xD2,
    0xE8, W, W, W, W,
    0x48, 0x8B, 0xCE
};

typedef struct {
    const WCHAR* name;
    const WORD* bytes;
    int len;
    int valOff[4];
    int nVal;
    int expect;
} Pattern;

static Pattern s_pats[] = {
    { L"Alloc x3",  PAT_ALLOC, sizeof(PAT_ALLOC)/sizeof(WORD), {1,18,35}, 3, 1 },
    { L"Check 1&3", PAT_CHK13, sizeof(PAT_CHK13)/sizeof(WORD), {5},       1, 2 },
    { L"Check 2",   PAT_CHK2,  sizeof(PAT_CHK2)/sizeof(WORD),  {5},       1, 1 },
    { L"Memset",    PAT_MSET,  sizeof(PAT_MSET)/sizeof(WORD),  {5},       1, 1 },
};
#define PAT_COUNT (sizeof(s_pats) / sizeof(s_pats[0]))

/* Global state */

PatcherState g_patch;

/* Pattern scan */

static BOOL match_at(const BYTE* data, const WORD* pat, int len)
{
    int i;
    for (i = 0; i < len; i++)
        if (pat[i] != W && data[i] != (BYTE)pat[i])
            return FALSE;
    return TRUE;
}

static int scan_patterns(void)
{
    int p, i, v;

    g_patch.count   = 0;
    g_patch.scanErr = 0;

    for (p = 0; p < (int)PAT_COUNT; p++) {
        Pattern* pat = &s_pats[p];
        int hits = 0;

        for (i = 0; (DWORD)(i + pat->len) <= g_patch.fileSize; i++) {
            if (match_at(g_patch.data + i, pat->bytes, pat->len)) {
                for (v = 0; v < pat->nVal && g_patch.count < PATCH_MAX_PTS; v++)
                    g_patch.offsets[g_patch.count++] = i + pat->valOff[v];
                hits++;
                i += pat->len - 1;
            }
        }

        if (hits != pat->expect) {
            g_patch.scanErr = -(p + 1);
            return g_patch.scanErr;
        }
    }

    /* verify all values are identical */
    g_patch.origVal = *(DWORD*)(g_patch.data + g_patch.offsets[0]);
    g_patch.curVal  = g_patch.origVal;
    for (i = 1; i < g_patch.count; i++) {
        if (*(DWORD*)(g_patch.data + g_patch.offsets[i]) != g_patch.origVal) {
            g_patch.scanErr = -99;
            return g_patch.scanErr;
        }
    }

    return g_patch.count;
}

/* File I/O */

BOOL patcher_load(const WCHAR* path)
{
    HANDLE h;
    DWORD rd = 0;

    if (g_patch.data) { 
        free(g_patch.data);
        g_patch.data = NULL;
    }
    g_patch.loaded  = FALSE;
    g_patch.count   = 0;
    g_patch.scanErr = 0;

    /* store path immediately so UI always reflects the selection */
    wcsncpy(g_patch.path, path, MAX_PATH - 1);
    g_patch.path[MAX_PATH - 1] = L'\0';

    h = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
        return FALSE;

    g_patch.fileSize = GetFileSize(h, NULL);
    if (g_patch.fileSize < 0x100000) {
        CloseHandle(h);
        return FALSE;
    }

    g_patch.data = (BYTE*)malloc(g_patch.fileSize);
    if (!g_patch.data) {
        CloseHandle(h);
        return FALSE; 
    }

    BOOL r = ReadFile(h, g_patch.data, g_patch.fileSize, &rd, NULL);
    CloseHandle(h);

    if (!r || rd != g_patch.fileSize) {
        free(g_patch.data); g_patch.data = NULL;
        return FALSE;
    }

    g_patch.loaded = TRUE;

    scan_patterns();
    return TRUE;
}

BOOL patcher_write(DWORD newVal)
{
    WCHAR bak[MAX_PATH + 8];
    HANDLE h;
    BOOL ok = TRUE;
    int i;

    /* backup on first write */
    swprintf(bak, MAX_PATH + 8, L"%s.bak", g_patch.path);
    CopyFileW(g_patch.path, bak, TRUE);

    h = CreateFileW(g_patch.path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return FALSE;

    for (i = 0; i < g_patch.count; i++) {
        DWORD wr = 0;
        SetFilePointer(h, (LONG)g_patch.offsets[i], NULL, FILE_BEGIN);
        if (!WriteFile(h, &newVal, 4, &wr, NULL) || wr != 4)
            ok = FALSE;
        *(DWORD*)(g_patch.data + g_patch.offsets[i]) = newVal;
    }
    CloseHandle(h);

    if (ok) g_patch.curVal = newVal;
    return ok;
}

/* Auto-detect via KnownFolders  */

static BOOL search_in_dir(const WCHAR* dir, WCHAR* out, int outLen)
{
    swprintf(out, outLen, L"%s\\RAWMHUB\\RAWMHUB.exe", dir);
    return GetFileAttributesW(out) != INVALID_FILE_ATTRIBUTES;
}

BOOL patcher_auto_detect(WCHAR* out, int outLen)
{
    static const KNOWNFOLDERID* folders[] = {
        &FOLDERID_ProgramFiles,
        &FOLDERID_ProgramFilesX86,
    };
    int i;

    for (i = 0; i < 2; i++) {
        WCHAR* folderPath = NULL;
        if (SUCCEEDED(SHGetKnownFolderPath(folders[i], 0, NULL, &folderPath))) {
            BOOL found = search_in_dir(folderPath, out, outLen);
            CoTaskMemFree(folderPath);
            if (found) return TRUE;
        }
    }
    return FALSE;
}

/* Format helper */

const WCHAR* patcher_fmt_size(DWORD v, WCHAR* buf, int len)
{
    if (v >= 0x100000)
        swprintf(buf, len, L"0x%X (%.1f MB)", v, v / 1048576.0);
    else if (v >= 0x400)
        swprintf(buf, len, L"0x%X (%u KB)", v, v / 1024);
    else
        swprintf(buf, len, L"0x%X (%u B)", v, v);
    return buf;
}
