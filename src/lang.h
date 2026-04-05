/*
 * lang.h - localization string table
 *
 * Auto-detects language via GetUserDefaultUILanguage().
 * Call lang_init() once at startup.
 * Use S(STR_XXX) to get the localized string.
 */

#pragma once
#include <windows.h>

/* Repo */

#define APP_REPO    L"https://github.com/pr701/rawm-patcher"

/* String IDs */

enum {
    /* UI labels & buttons */
    STR_WINDOW_TITLE,
    STR_FILE,
    STR_SIZE,
    STR_PATCH,
    STR_RESTORE,
    STR_ABOUT,

    /* status */
    STR_NO_FILE,
    STR_ERR_VALUES_DIFFER,
    STR_ERR_PATTERN_FMT,    /* "Found %d/%d ..." */
    STR_INCOMPATIBLE,
    STR_FOUND_FMT,          /* "Found %d/%d patch points" */
    STR_CUR_SIZE_FMT,       /* "Current buffer size: %s" */

    /* patch flow */
    STR_INVALID_SIZE,
    STR_CONFIRM_FMT,        /* "Patch %d locations to %s? ..." */
    STR_CONFIRM_TITLE,
    STR_DONE_FMT,           /* "Patched %d locations ..." */
    STR_DONE_TITLE,
    STR_WRITE_FAIL,
    STR_RESTORED_FMT,       /* "Restored to %s" */
    STR_RESTORED_TITLE,

    /* about */
    STR_ABOUT_FMT,
    STR_ABOUT_TITLE,

    /* process */
    STR_PROC_RUNNING,
    STR_PROC_TITLE,
    STR_PROC_KILL_FAIL,

    /* file dialog */
    STR_FILTER_EXE,
    STR_FILTER_ALL,

    /* error title */
    STR_ERROR,

    STR__COUNT
};

void lang_init(int lcid);  /* 0 = auto, or LANGID override */
const WCHAR* lang_get(int id);

#define S(id) lang_get(id)
