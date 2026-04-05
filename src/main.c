/*
 * main.c - entry point, window creation, message loop
 *
 * RAWM Buffer Patcher
 * Fixes ~2 GB memory leak in RAWMHUB.exe by patching buffer allocation size.
 * Tested Build: Visual Studio 2022, x64, Windows subsystem, Unicode
 */

#include "ui.h"
#include "lang.h"
#include <shellapi.h>
#include <stdlib.h>

/* inline manifet for styles
#pragma comment(linker, "\"/manifestdependency:type='win32' "                  \
    "name='Microsoft.Windows.Common-Controls' version='6.0.0.0' "              \
    "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") */

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPWSTR lpCmd, int nShow)
{
    WNDCLASSEXW wc;
    DWORD style;
    RECT rc;
    HWND hwnd;
    int dpi, argc;
    LPWSTR *argv;
    WCHAR autoPath[MAX_PATH] = L"";
    MSG msg;

    int langOverride = 0;
    int i;

    (void)hPrev;
    (void)lpCmd;

    /* parse command-line args: --lcid N and file path */
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv) {
        for (i = 1; i < argc; i++) {
            if ((_wcsicmp(argv[i], L"--lang") == 0 || _wcsicmp(argv[i], L"-lang") == 0)
                && i + 1 < argc) {
                langOverride = (int)wcstol(argv[i + 1], NULL, 0);
                i++;
            } else if (GetFileAttributesW(argv[i]) != INVALID_FILE_ATTRIBUTES) {
                wcsncpy(autoPath, argv[i], MAX_PATH - 1);
            }
        }
    }

    /* init localization before any UI */
    lang_init(langOverride);

    if (argv) LocalFree(argv);

    /* DPI awareness (fallback for systems without manifest) */
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    /* register window class */
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = ui_wndproc;
    wc.hInstance = hInst;
    wc.hIcon = LoadIconW(hInst, L"IDI_APP");
    wc.hIconSm = LoadIconW(hInst, L"IDI_APP");
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"RAWMPatcher";
    RegisterClassExW(&wc);

    /* window size */
    style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    rc.left = 0; rc.top = 0; rc.right = 455; rc.bottom = 130;
    AdjustWindowRectExForDpi(&rc, style, FALSE, 0, GetDpiForSystem());

    {
        int ww = rc.right - rc.left;
        int wh = rc.bottom - rc.top;
        int sx = GetSystemMetrics(SM_CXSCREEN);
        int sy = GetSystemMetrics(SM_CYSCREEN);
        hwnd = CreateWindowExW(0, L"RAWMPatcher", S(STR_WINDOW_TITLE), style,
            (sx - ww) / 2, (sy - wh) / 2, ww, wh,
            NULL, NULL, hInst, NULL);
    }

    /* create controls */
    dpi = (int)GetDpiForWindow(hwnd);
    ui_create_controls(hwnd, hInst, dpi);

    /* auto-detect if no file from command line */
    if (!autoPath[0])
        patcher_auto_detect(autoPath, MAX_PATH);

    if (autoPath[0])
        patcher_load(autoPath);
    ui_update(hwnd);

    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);

    /* message loop */
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (g_patch.data) free(g_patch.data);
    return (int)msg.wParam;
}
