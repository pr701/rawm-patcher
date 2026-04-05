/*
 * process.h - find and kill target process by executable name
 */
#pragma once
#include <windows.h>

/*
 * process_check_running
 *   Checks if a process with the given exe name is running.
 *   Returns TRUE if found, FALSE otherwise.
 */
BOOL process_check_running(const WCHAR* exeName);

/*
 * process_kill
 *   Terminates all instances of a process by exe name.
 *   Returns TRUE if all were terminated (or none found).
 */
BOOL process_kill(const WCHAR* exeName);

/*
 * process_prompt_kill
 *   If the process is running, shows a warning dialog.
 *   User can choose to kill it or cancel.
 *   Returns TRUE if process is not running (or was killed).
 *   Returns FALSE if user cancelled.
 */
BOOL process_prompt_kill(HWND hwnd, const WCHAR* exeName);
