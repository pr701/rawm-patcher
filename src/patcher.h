/*
 * patcher.h - pattern scanner, file I/O, auto-detect
 */

#pragma once
#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#define PATCH_MAX_PTS 16
#define PATCH_REC_SIZE 0x10000 /* recommended: 64 KB */
#define PATCH_EXPECT_PTS 7

/* Patcher state */

typedef struct {
    WCHAR path[MAX_PATH];
    BYTE* data;
    DWORD fileSize;
    DWORD offsets[PATCH_MAX_PTS];
    int count;      /* found patch points */
    DWORD origVal;  /* value when file was loaded */
    DWORD curVal;   /* current value (tracks patches) */
    int scanErr;    /* 0 = ok, <0 = error code */
    BOOL loaded;
} PatcherState;

extern PatcherState g_patch;

BOOL patcher_load(const WCHAR* path);
BOOL patcher_write(DWORD newVal);
BOOL patcher_auto_detect(WCHAR* out, int outLen);
const WCHAR* patcher_fmt_size(DWORD v, WCHAR* buf, int len);
