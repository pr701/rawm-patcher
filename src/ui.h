/*
 * ui.h - window procedure, controls, UI updates
 */
#pragma once
#include "patcher.h"

/* Control IDs */

#define IDC_PATH    101
#define IDC_BROWSE  102
#define IDC_STATUS  103
#define IDC_CURVAL  104
#define IDC_SIZEED  105
#define IDC_PATCH   106
#define IDC_RESTOR  107
#define IDC_ABOUT   108

void ui_create_controls(HWND hwnd, HINSTANCE hInst, int dpi);
void ui_update(HWND hwnd);
LRESULT CALLBACK ui_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
