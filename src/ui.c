/*
 * ui.c - window procedure, controls, UI updates
 */
#include "ui.h"
#include "lang.h"
#include "process.h"
#include <commdlg.h>
#include <shellapi.h>
#include <stdio.h>
#include <wchar.h>

#pragma comment(lib, "comdlg32.lib")

/* Colors */

#define CLR_OK      RGB(0, 140, 0)
#define CLR_ERR     RGB(200, 0, 0)
#define CLR_DEFAULT GetSysColor(COLOR_WINDOWTEXT)

static COLORREF s_statusColor = 0;

/* Fonts */

static HFONT s_fontNormal;
static HFONT s_fontInfo;

/* Control helpers */

static HWND mk_label(HINSTANCE hi, HFONT f, HWND parent,
                      int id, int x, int y, int w, int h, const WCHAR* t)
{
    HWND c = CreateWindowExW(0, L"STATIC", t,
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        x, y, w, h, parent, (HMENU)(INT_PTR)id, hi, NULL);
    SendMessageW(c, WM_SETFONT, (WPARAM)f, 0);
    return c;
}

static HWND mk_edit(HINSTANCE hi, HFONT f, HWND parent,
                     int id, int x, int y, int w, int h, DWORD extra)
{
    HWND c = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | extra,
        x, y, w, h, parent, (HMENU)(INT_PTR)id, hi, NULL);
    SendMessageW(c, WM_SETFONT, (WPARAM)f, 0);
    return c;
}

static HWND mk_btn(HINSTANCE hi, HFONT f, HWND parent,
                    int id, int x, int y, int w, int h, const WCHAR* t)
{
    HWND c = CreateWindowExW(0, L"BUTTON", t,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, w, h, parent, (HMENU)(INT_PTR)id, hi, NULL);
    SendMessageW(c, WM_SETFONT, (WPARAM)f, 0);
    return c;
}

/* Create controls */

void ui_create_controls(HWND hwnd, HINSTANCE hInst, int dpi)
{
    int fontH = -MulDiv(9, dpi, 72);

    s_fontNormal = CreateFontW(fontH, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    s_fontInfo = CreateFontW(fontH, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");

    s_statusColor = CLR_DEFAULT;

    /* Row 0: file path + browse */
    mk_label(hInst, s_fontNormal, hwnd, 0,          12, 12, 38, 16, S(STR_FILE));
    mk_edit (hInst, s_fontNormal, hwnd, IDC_PATH,   52, 9, 338, 23, ES_READONLY);
    mk_btn  (hInst, s_fontNormal, hwnd, IDC_BROWSE, 394, 9, 42, 23, L"...");

    /* Row 1-2: status + current value */
    mk_label(hInst, s_fontInfo, hwnd, IDC_STATUS, 12, 38, 424, 18, L"");
    mk_label(hInst, s_fontInfo, hwnd, IDC_CURVAL, 12, 58, 424, 18, L"");

    /* Row 3: size input + patch/restore/about buttons */
    mk_label(hInst, s_fontNormal, hwnd, 0,          12,  86, 42, 16, S(STR_SIZE));
    mk_edit (hInst, s_fontNormal, hwnd, IDC_SIZEED, 56,  83, 110, 23, 0);
    mk_btn  (hInst, s_fontNormal, hwnd, IDC_PATCH,  174, 82, 70, 26, S(STR_PATCH));
    mk_btn  (hInst, s_fontNormal, hwnd, IDC_RESTOR, 250, 82, 70, 26, S(STR_RESTORE));
    mk_btn  (hInst, s_fontNormal, hwnd, IDC_ABOUT,  366, 82, 70, 26, S(STR_ABOUT));

    /* defaults */
    SetDlgItemTextW(hwnd, IDC_SIZEED, L"0x10000");
    EnableWindow(GetDlgItem(hwnd, IDC_PATCH),  FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_RESTOR), FALSE);
}

/* ═══════════════════════════════════════════════════════════════════
   UI update
   ═══════════════════════════════════════════════════════════════════ */

void ui_update(HWND hwnd)
{
    WCHAR buf[256], sz[64];

    SetDlgItemTextW(hwnd, IDC_PATH, g_patch.path);

    if (!g_patch.loaded) {
        s_statusColor = CLR_DEFAULT;
        SetDlgItemTextW(hwnd, IDC_STATUS, S(STR_NO_FILE));
        SetDlgItemTextW(hwnd, IDC_CURVAL, L"");
        EnableWindow(GetDlgItem(hwnd, IDC_PATCH),  FALSE);
        EnableWindow(GetDlgItem(hwnd, IDC_RESTOR), FALSE);
        InvalidateRect(GetDlgItem(hwnd, IDC_STATUS), NULL, TRUE);
        return;
    }

    if (g_patch.scanErr) {
        s_statusColor = CLR_ERR;
        if (g_patch.scanErr == -99)
            swprintf(buf, 256, L"%s", S(STR_ERR_VALUES_DIFFER));
        else
            swprintf(buf, 256, S(STR_ERR_PATTERN_FMT),
                g_patch.count, PATCH_EXPECT_PTS);
        SetDlgItemTextW(hwnd, IDC_STATUS, buf);
        SetDlgItemTextW(hwnd, IDC_CURVAL, S(STR_INCOMPATIBLE));
        EnableWindow(GetDlgItem(hwnd, IDC_PATCH),  FALSE);
        EnableWindow(GetDlgItem(hwnd, IDC_RESTOR), FALSE);
        InvalidateRect(GetDlgItem(hwnd, IDC_STATUS), NULL, TRUE);
        return;
    }

    /* all found */
    s_statusColor = CLR_OK;
    swprintf(buf, 256, S(STR_FOUND_FMT), g_patch.count, PATCH_EXPECT_PTS);
    SetDlgItemTextW(hwnd, IDC_STATUS, buf);

    patcher_fmt_size(g_patch.curVal, sz, 64);
    swprintf(buf, 256, S(STR_CUR_SIZE_FMT), sz);
    SetDlgItemTextW(hwnd, IDC_CURVAL, buf);

    EnableWindow(GetDlgItem(hwnd, IDC_PATCH),  TRUE);
    EnableWindow(GetDlgItem(hwnd, IDC_RESTOR), g_patch.curVal != g_patch.origVal);

    InvalidateRect(GetDlgItem(hwnd, IDC_STATUS), NULL, TRUE);
}

/* Actions */

static void on_browse(HWND hwnd)
{
    WCHAR file[MAX_PATH] = L"";
    WCHAR filter[128];
    int   pos = 0;
    OPENFILENAMEW ofn;

    /* build double-null-terminated filter from localized strings */
    pos += swprintf(filter + pos, 128 - pos, L"%s (exe)", S(STR_FILTER_EXE)) + 1;
    pos += swprintf(filter + pos, 128 - pos, L"*.exe") + 1;
    pos += swprintf(filter + pos, 128 - pos, L"%s", S(STR_FILTER_ALL)) + 1;
    pos += swprintf(filter + pos, 128 - pos, L"*.*") + 1;
    filter[pos] = L'\0';

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = file;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        patcher_load(file);
        ui_update(hwnd);
    }
}

static DWORD parse_hex_input(HWND hwnd)
{
    WCHAR buf[32];
    WCHAR *p, *end;

    GetDlgItemTextW(hwnd, IDC_SIZEED, buf, 32);

    p = buf;
    if (p[0] == L'0' && (p[1] == L'x' || p[1] == L'X'))
        p += 2;

    end = NULL;
    return wcstoul(p, &end, 16);
}

static void on_patch(HWND hwnd)
{
    DWORD val;
    WCHAR msg[256], sz[64];
    int   ret;

    if (g_patch.scanErr || g_patch.count == 0) return;

    val = parse_hex_input(hwnd);

    if (val == 0 || val > 0x1000000) {
        MessageBoxW(hwnd, S(STR_INVALID_SIZE), S(STR_ERROR), MB_ICONERROR);
        return;
    }

    /* check if target is running */
    {
        const WCHAR* fname = wcsrchr(g_patch.path, L'\\');
        fname = fname ? fname + 1 : g_patch.path;
        if (!process_prompt_kill(hwnd, fname))
            return;
    }

    /* confirmation */
    patcher_fmt_size(val, sz, 64);
    swprintf(msg, 256, S(STR_CONFIRM_FMT), g_patch.count, sz);

    ret = MessageBoxW(hwnd, msg, S(STR_CONFIRM_TITLE), MB_ICONWARNING | MB_OKCANCEL);
    if (ret != IDOK) return;

    if (patcher_write(val)) {
        patcher_fmt_size(val, sz, 64);
        swprintf(msg, 256, S(STR_DONE_FMT), g_patch.count, sz, g_patch.path);
        MessageBoxW(hwnd, msg, S(STR_DONE_TITLE), MB_ICONINFORMATION);
        ui_update(hwnd);
    } else {
        MessageBoxW(hwnd, S(STR_WRITE_FAIL), S(STR_ERROR), MB_ICONERROR);
    }
}

static void on_restore(HWND hwnd)
{
    WCHAR msg[128], sz[64];

    if (g_patch.scanErr || g_patch.count == 0) return;

    if (patcher_write(g_patch.origVal)) {
        patcher_fmt_size(g_patch.origVal, sz, 64);
        swprintf(msg, 128, S(STR_RESTORED_FMT), sz);
        MessageBoxW(hwnd, msg, S(STR_RESTORED_TITLE), MB_ICONINFORMATION);
        ui_update(hwnd);
    } else {
        MessageBoxW(hwnd, S(STR_WRITE_FAIL), S(STR_ERROR), MB_ICONERROR);
    }
}

static void on_about(HWND hwnd)
{
    MessageBoxW(hwnd, S(STR_ABOUT_FMT), S(STR_ABOUT_TITLE), MB_ICONINFORMATION);
}

/* dispatcher */

LRESULT CALLBACK ui_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDC_BROWSE:
            on_browse(hwnd); break;
        case IDC_PATCH:
            on_patch(hwnd); break;
        case IDC_RESTOR:
            on_restore(hwnd); break;
        case IDC_ABOUT:
            on_about(hwnd); break;
        }
        return 0;

    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wp;
        HWND ctl = (HWND)lp;
        int id  = GetDlgCtrlID(ctl);

        SetBkMode(hdc, TRANSPARENT);

        if (id == IDC_STATUS)
            SetTextColor(hdc, s_statusColor);
        else
            SetTextColor(hdc, CLR_DEFAULT);

        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
    }

    case WM_DESTROY:
        if (s_fontNormal)
            DeleteObject(s_fontNormal);
        if (s_fontInfo)
            DeleteObject(s_fontInfo);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}
