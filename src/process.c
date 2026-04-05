/*
 * process.c - find and kill target process by executable name
 */
#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "process.h"
#include "lang.h"
#include <tlhelp32.h>
#include <wchar.h>

BOOL process_check_running(const WCHAR* exeName)
{
    HANDLE snap;
    PROCESSENTRY32W pe;
    BOOL found = FALSE;

    snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return FALSE;

    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, exeName) == 0) {
                found = TRUE;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }

    CloseHandle(snap);
    return found;
}

BOOL process_kill(const WCHAR* exeName)
{
    HANDLE snap, proc;
    PROCESSENTRY32W pe;
    BOOL ok = TRUE;

    snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return FALSE;

    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, exeName) == 0) {
                proc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (proc) {
                    if (!TerminateProcess(proc, 1))
                        ok = FALSE;
                    CloseHandle(proc);
                } else {
                    ok = FALSE;
                }
            }
        } while (Process32NextW(snap, &pe));
    }

    CloseHandle(snap);
    return ok;
}

BOOL process_prompt_kill(HWND hwnd, const WCHAR* exeName)
{
    int ret;

    if (!process_check_running(exeName))
        return TRUE;

    ret = MessageBoxW(hwnd, S(STR_PROC_RUNNING), S(STR_PROC_TITLE),
        MB_ICONWARNING | MB_YESNO);

    if (ret != IDYES)
        return FALSE;

    if (!process_kill(exeName)) {
        MessageBoxW(hwnd, S(STR_PROC_KILL_FAIL), S(STR_ERROR), MB_ICONERROR);
        return FALSE;
    }

    Sleep(500);  /* let OS release file handles */
    return TRUE;
}
