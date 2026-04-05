/*
 * lang.c - localization string table
 */

#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "lang.h"

/* Languages indx */

enum {
    LANG_IDX_EN,
    LANG_IDX_RU,
    LANG_IDX_UK,
    LANG_IDX_ZH_CN,
    LANG_IDX_ZH_TW,
    LANG_IDX_DE,
    LANG_IDX_PT,
    LANG__COUNT
};

static int s_lang = LANG_IDX_EN;

/* String table */

static const WCHAR* s_strings[LANG__COUNT][STR__COUNT] = {

/* English */

[LANG_IDX_EN] = {

    [STR_WINDOW_TITLE]  = L"RAWM Buffer Patcher",
    [STR_FILE]          = L"File:",
    [STR_SIZE]          = L"Size:",
    [STR_PATCH]         = L"Patch",
    [STR_RESTORE]       = L"Restore",
    [STR_ABOUT]         = L"About",

    [STR_NO_FILE]           = L"No file loaded. Click [...] to browse.",
    [STR_ERR_VALUES_DIFFER] = L"Error: values at patch points differ!",
    [STR_ERR_PATTERN_FMT]   = L"Found %d/%d patch points — pattern mismatch",
    [STR_INCOMPATIBLE]      = L"Incompatible binary version.",
    [STR_FOUND_FMT]         = L"Found %d/%d patch points",
    [STR_CUR_SIZE_FMT]      = L"Current buffer size: %s",

    [STR_INVALID_SIZE]  = L"Invalid size.\n\n"
                          L"Enter a hex value from 0x1 to 0x1000000 (16 MB).\n"
                          L"Recommended: 0x10000 (64 KB).",
    [STR_CONFIRM_FMT]   = L"Patch %d locations to %s?\n\n"
                          L"A backup (.bak) will be created.",
    [STR_CONFIRM_TITLE] = L"Confirm patch",
    [STR_DONE_FMT]      = L"Patched %d locations.\n"
                          L"New buffer size: %s\n\n"
                          L"Backup: %s.bak",
    [STR_DONE_TITLE]    = L"Done",
    [STR_WRITE_FAIL]    = L"Failed to write file.\n\n"
                          L"Make sure RAWMHUB is not running.",
    [STR_RESTORED_FMT]  = L"Restored to %s",
    [STR_RESTORED_TITLE]= L"Restored",

    [STR_ABOUT_FMT]     = L"RAWM Buffer Patcher\n\n"
                          L"Repository: " APP_REPO,
    [STR_ABOUT_TITLE]   = L"About",

    [STR_PROC_RUNNING]  = L"RAWMHUB is currently running.\n\n"
                          L"It must be closed before patching.\n"
                          L"Terminate the process now?",
    [STR_PROC_TITLE]    = L"Process running",
    [STR_PROC_KILL_FAIL]= L"Failed to terminate the process.\n"
                          L"Try running the patcher as Administrator.",

    [STR_FILTER_EXE]    = L"Executable",
    [STR_FILTER_ALL]    = L"All files",
    [STR_ERROR]         = L"Error",
},

/* Русский */

[LANG_IDX_RU] = {

    [STR_WINDOW_TITLE]  = L"RAWM Buffer Patcher",
    [STR_FILE]          = L"Файл:",
    [STR_SIZE]          = L"Разм.:",
    [STR_PATCH]         = L"Патч",
    [STR_RESTORE]       = L"Восст.",
    [STR_ABOUT]         = L"О прог.",

    [STR_NO_FILE]           = L"Файл не загружен. Нажмите [...] для выбора.",
    [STR_ERR_VALUES_DIFFER] = L"Ошибка: значения в точках патча различаются!",
    [STR_ERR_PATTERN_FMT]   = L"Найдено %d/%d точек — несовпадение",
    [STR_INCOMPATIBLE]      = L"Несовместимая версия.",
    [STR_FOUND_FMT]         = L"Найдено %d/%d точек патча",
    [STR_CUR_SIZE_FMT]      = L"Текущий размер: %s",

    [STR_INVALID_SIZE]  = L"Неверный размер.\n\n"
                          L"Введите hex значение от 0x1 до 0x1000000 (16 MB).\n"
                          L"Рекомендуется: 0x10000 (64 KB).",
    [STR_CONFIRM_FMT]   = L"Пропатчить %d точек, размер: %s?\n\n"
                          L"Бэкап (.bak) будет создан.",
    [STR_CONFIRM_TITLE] = L"Подтверждение",
    [STR_DONE_FMT]      = L"Пропатчено %d точек.\n"
                          L"Новый размер: %s\n\n"
                          L"Бэкап: %s.bak",
    [STR_DONE_TITLE]    = L"Готово",
    [STR_WRITE_FAIL]    = L"Не удалось записать файл.\n\n"
                          L"Убедитесь, что RAWMHUB не запущен.",
    [STR_RESTORED_FMT]  = L"Восстановлено: %s",
    [STR_RESTORED_TITLE]= L"Восстановлено",

    [STR_ABOUT_FMT]     = L"RAWM Buffer Patcher\n\n"
                          L"Репозиторий: " APP_REPO,
    [STR_ABOUT_TITLE]   = L"О программе",

    [STR_PROC_RUNNING]  = L"RAWMHUB сейчас запущен.\n\n"
                          L"Для патча его нужно закрыть.\n"
                          L"Завершить процесс?",
    [STR_PROC_TITLE]    = L"Процесс запущен",
    [STR_PROC_KILL_FAIL]= L"Не удалось завершить процесс.\n"
                          L"Попробуйте запустить от имени Администратора.",

    [STR_FILTER_EXE]    = L"Исполняемые",
    [STR_FILTER_ALL]    = L"Все файлы",
    [STR_ERROR]         = L"Ошибка",
},

/* Українська */

[LANG_IDX_UK] = {

    [STR_WINDOW_TITLE]  = L"RAWM Buffer Patcher",
    [STR_FILE]          = L"Файл:",
    [STR_SIZE]          = L"Розм.:",
    [STR_PATCH]         = L"Патч",
    [STR_RESTORE]       = L"Відн.",
    [STR_ABOUT]         = L"Про...",

    [STR_NO_FILE]           = L"Файл не завантажено. Натисніть [...] для вибору.",
    [STR_ERR_VALUES_DIFFER] = L"Помилка: значення в точках патча відрізняються!",
    [STR_ERR_PATTERN_FMT]   = L"Знайдено %d/%d точок — невідповідність",
    [STR_INCOMPATIBLE]      = L"Несумісна версія.",
    [STR_FOUND_FMT]         = L"Знайдено %d/%d точок патча",
    [STR_CUR_SIZE_FMT]      = L"Поточний розмір: %s",

    [STR_INVALID_SIZE]  = L"Невірний розмір.\n\n"
                          L"Введіть hex значення від 0x1 до 0x1000000 (16 MB).\n"
                          L"Рекоменд.: 0x10000 (64 KB).",
    [STR_CONFIRM_FMT]   = L"Пропатчити %d точок, розмір: %s?\n\n"
                          L"Бекап (.bak) буде створено.",
    [STR_CONFIRM_TITLE] = L"Підтвердження",
    [STR_DONE_FMT]      = L"Пропатчено %d точок.\n"
                          L"Новий розмір: %s\n\n"
                          L"Бекап: %s.bak",
    [STR_DONE_TITLE]    = L"Готово",
    [STR_WRITE_FAIL]    = L"Не вдалося записати файл.\n\n"
                          L"Переконайтеся, що RAWMHUB не запущено.",
    [STR_RESTORED_FMT]  = L"Відновлено: %s",
    [STR_RESTORED_TITLE]= L"Відновлено",

    [STR_ABOUT_FMT]     = L"RAWM Buffer Patcher\n\n"
                          L"Репозиторій: " APP_REPO,
    [STR_ABOUT_TITLE]   = L"Про програму",

    [STR_PROC_RUNNING]  = L"RAWMHUB зараз запущено.\n\n"
                          L"Для патча його потрібно закрити.\n"
                          L"Завершити процес?",
    [STR_PROC_TITLE]    = L"Процес запущено",
    [STR_PROC_KILL_FAIL]= L"Не вдалося завершити процес.\n"
                          L"Спробуйте запустити від імені Адміністратора.",

    [STR_FILTER_EXE]    = L"Виконувані",
    [STR_FILTER_ALL]    = L"Усі файли",
    [STR_ERROR]         = L"Помилка",
},

/* 简体中文 */

[LANG_IDX_ZH_CN] = {

    [STR_WINDOW_TITLE]  = L"RAWM 缓冲区补丁",
    [STR_FILE]          = L"文件:",
    [STR_SIZE]          = L"大小:",
    [STR_PATCH]         = L"补丁",
    [STR_RESTORE]       = L"还原",
    [STR_ABOUT]         = L"关于",

    [STR_NO_FILE]           = L"未加载文件。点击 [...] 浏览。",
    [STR_ERR_VALUES_DIFFER] = L"错误: 补丁点的值不一致!",
    [STR_ERR_PATTERN_FMT]   = L"找到 %d/%d 个补丁点 — 模式不匹配",
    [STR_INCOMPATIBLE]      = L"不兼容的版本。",
    [STR_FOUND_FMT]         = L"找到 %d/%d 个补丁点",
    [STR_CUR_SIZE_FMT]      = L"当前缓冲区大小: %s",

    [STR_INVALID_SIZE]  = L"无效大小。\n\n"
                          L"请输入 0x1 到 0x1000000 (16 MB) 的十六进制值。\n"
                          L"建议: 0x10000 (64 KB)。",
    [STR_CONFIRM_FMT]   = L"补丁 %d 个位置，新大小: %s?\n\n"
                          L"将创建备份 (.bak)。",
    [STR_CONFIRM_TITLE] = L"确认补丁",
    [STR_DONE_FMT]      = L"已补丁 %d 个位置。\n"
                          L"新大小: %s\n\n"
                          L"备份: %s.bak",
    [STR_DONE_TITLE]    = L"完成",
    [STR_WRITE_FAIL]    = L"无法写入文件。\n\n"
                          L"请确保 RAWMHUB 未运行。",
    [STR_RESTORED_FMT]  = L"已还原: %s",
    [STR_RESTORED_TITLE]= L"已还原",

    [STR_ABOUT_FMT]     = L"RAWM Buffer Patcher\n\n"
                          L"仓库: " APP_REPO,
    [STR_ABOUT_TITLE]   = L"关于",

    [STR_PROC_RUNNING]  = L"RAWMHUB 正在运行。\n\n"
                          L"补丁前需要关闭它。\n"
                          L"终止进程？",
    [STR_PROC_TITLE]    = L"进程运行中",
    [STR_PROC_KILL_FAIL]= L"无法终止进程。\n"
                          L"请以管理员身份运行。",

    [STR_FILTER_EXE]    = L"可执行文件",
    [STR_FILTER_ALL]    = L"所有文件",
    [STR_ERROR]         = L"错误",
},

/* 繁體中文 */

[LANG_IDX_ZH_TW] = {

    [STR_WINDOW_TITLE]  = L"RAWM 緩衝區補丁",
    [STR_FILE]          = L"檔案:",
    [STR_SIZE]          = L"大小:",
    [STR_PATCH]         = L"補丁",
    [STR_RESTORE]       = L"還原",
    [STR_ABOUT]         = L"關於",

    [STR_NO_FILE]           = L"未載入檔案。點擊 [...] 瀏覽。",
    [STR_ERR_VALUES_DIFFER] = L"錯誤: 補丁點的值不一致!",
    [STR_ERR_PATTERN_FMT]   = L"找到 %d/%d 個補丁點 — 模式不匹配",
    [STR_INCOMPATIBLE]      = L"不相容的版本。",
    [STR_FOUND_FMT]         = L"找到 %d/%d 個補丁點",
    [STR_CUR_SIZE_FMT]      = L"目前緩衝區大小: %s",

    [STR_INVALID_SIZE]  = L"無效大小。\n\n"
                          L"請輸入 0x1 到 0x1000000 (16 MB) 的十六進位值。\n"
                          L"建議: 0x10000 (64 KB)。",
    [STR_CONFIRM_FMT]   = L"補丁 %d 個位置，新大小: %s?\n\n"
                          L"將建立備份 (.bak)。",
    [STR_CONFIRM_TITLE] = L"確認補丁",
    [STR_DONE_FMT]      = L"已補丁 %d 個位置。\n"
                          L"新大小: %s\n\n"
                          L"備份: %s.bak",
    [STR_DONE_TITLE]    = L"完成",
    [STR_WRITE_FAIL]    = L"無法寫入檔案。\n\n"
                          L"請確保 RAWMHUB 未執行。",
    [STR_RESTORED_FMT]  = L"已還原: %s",
    [STR_RESTORED_TITLE]= L"已還原",

    [STR_ABOUT_FMT]     = L"RAWM Buffer Patcher\n\n"
                          L"儲存庫: " APP_REPO,
    [STR_ABOUT_TITLE]   = L"關於",

    [STR_PROC_RUNNING]  = L"RAWMHUB 正在執行。\n\n"
                          L"補丁前需要關閉它。\n"
                          L"終止處理程序？",
    [STR_PROC_TITLE]    = L"處理程序執行中",
    [STR_PROC_KILL_FAIL]= L"無法終止處理程序。\n"
                          L"請以系統管理員身分執行。",

    [STR_FILTER_EXE]    = L"可執行檔",
    [STR_FILTER_ALL]    = L"所有檔案",
    [STR_ERROR]         = L"錯誤",
},

/* Deutsch */

[LANG_IDX_DE] = {

    [STR_WINDOW_TITLE]  = L"RAWM Buffer Patcher",
    [STR_FILE]          = L"Datei:",
    [STR_SIZE]          = L"Größe:",
    [STR_PATCH]         = L"Patch",
    [STR_RESTORE]       = L"Wied.",
    [STR_ABOUT]         = L"Über",

    [STR_NO_FILE]           = L"Keine Datei geladen. Klicken Sie [...] zum Durchsuchen.",
    [STR_ERR_VALUES_DIFFER] = L"Fehler: Werte an Patchstellen unterscheiden sich!",
    [STR_ERR_PATTERN_FMT]   = L"%d/%d Patchstellen gefunden — Muster stimmt nicht",
    [STR_INCOMPATIBLE]      = L"Inkompatible Version.",
    [STR_FOUND_FMT]         = L"%d/%d Patchstellen gefunden",
    [STR_CUR_SIZE_FMT]      = L"Aktuelle Puffergröße: %s",

    [STR_INVALID_SIZE]  = L"Ungültige Größe.\n\n"
                          L"Geben Sie einen Hex-Wert von 0x1 bis 0x1000000 (16 MB) ein.\n"
                          L"Empfohlen: 0x10000 (64 KB).",
    [STR_CONFIRM_FMT]   = L"%d Stellen auf %s patchen?\n\n"
                          L"Ein Backup (.bak) wird erstellt.",
    [STR_CONFIRM_TITLE] = L"Patch bestätigen",
    [STR_DONE_FMT]      = L"%d Stellen gepatcht.\n"
                          L"Neue Größe: %s\n\n"
                          L"Backup: %s.bak",
    [STR_DONE_TITLE]    = L"Fertig",
    [STR_WRITE_FAIL]    = L"Datei konnte nicht geschrieben werden.\n\n"
                          L"Stellen Sie sicher, dass RAWMHUB nicht läuft.",
    [STR_RESTORED_FMT]  = L"Wiederhergestellt: %s",
    [STR_RESTORED_TITLE]= L"Wiederhergestellt",

    [STR_ABOUT_FMT]     = L"RAWM Buffer Patcher\n\n"
                          L"Repository: " APP_REPO,
    [STR_ABOUT_TITLE]   = L"Über",

    [STR_PROC_RUNNING]  = L"RAWMHUB läuft gerade.\n\n"
                          L"Es muss vor dem Patchen geschlossen werden.\n"
                          L"Prozess beenden?",
    [STR_PROC_TITLE]    = L"Prozess läuft",
    [STR_PROC_KILL_FAIL]= L"Prozess konnte nicht beendet werden.\n"
                          L"Versuchen Sie, als Administrator zu starten.",

    [STR_FILTER_EXE]    = L"Ausführbare",
    [STR_FILTER_ALL]    = L"Alle Dateien",
    [STR_ERROR]         = L"Fehler",
},

/* Português (Brasil) */

[LANG_IDX_PT] = {

    [STR_WINDOW_TITLE]  = L"RAWM Buffer Patcher",
    [STR_FILE]          = L"Arq.:",
    [STR_SIZE]          = L"Tam.:",
    [STR_PATCH]         = L"Patch",
    [STR_RESTORE]       = L"Rest.",
    [STR_ABOUT]         = L"Sobre",

    [STR_NO_FILE]           = L"Nenhum arquivo carregado. Clique [...] para procurar.",
    [STR_ERR_VALUES_DIFFER] = L"Erro: valores nos pontos de patch diferem!",
    [STR_ERR_PATTERN_FMT]   = L"Encontrados %d/%d pontos — padrão não confere",
    [STR_INCOMPATIBLE]      = L"Versão incompatível.",
    [STR_FOUND_FMT]         = L"Encontrados %d/%d pontos de patch",
    [STR_CUR_SIZE_FMT]      = L"Tamanho atual do buffer: %s",

    [STR_INVALID_SIZE]  = L"Tamanho inválido.\n\n"
                          L"Insira um valor hex de 0x1 a 0x1000000 (16 MB).\n"
                          L"Recomendado: 0x10000 (64 KB).",
    [STR_CONFIRM_FMT]   = L"Aplicar patch em %d pontos para %s?\n\n"
                          L"Um backup (.bak) será criado.",
    [STR_CONFIRM_TITLE] = L"Confirmar patch",
    [STR_DONE_FMT]      = L"Patch aplicado em %d pontos.\n"
                          L"Novo tamanho: %s\n\n"
                          L"Backup: %s.bak",
    [STR_DONE_TITLE]    = L"Concluído",
    [STR_WRITE_FAIL]    = L"Falha ao gravar o arquivo.\n\n"
                          L"Certifique-se de que o RAWMHUB não está em execução.",
    [STR_RESTORED_FMT]  = L"Restaurado: %s",
    [STR_RESTORED_TITLE]= L"Restaurado",

    [STR_ABOUT_FMT]     = L"RAWM Buffer Patcher\n\n"
                          L"Repositório: " APP_REPO,
    [STR_ABOUT_TITLE]   = L"Sobre",

    [STR_PROC_RUNNING]  = L"RAWMHUB está em execução.\n\n"
                          L"Ele precisa ser fechado antes do patch.\n"
                          L"Encerrar o processo?",
    [STR_PROC_TITLE]    = L"Processo em execução",
    [STR_PROC_KILL_FAIL]= L"Falha ao encerrar o processo.\n"
                          L"Tente executar como Administrador.",

    [STR_FILTER_EXE]    = L"Executáveis",
    [STR_FILTER_ALL]    = L"Todos os arquivos",
    [STR_ERROR]         = L"Erro",
},

};

/* Implementation */

static int lang_from_id(LANGID lid)
{
    WORD primary = PRIMARYLANGID(lid);
    WORD sub     = SUBLANGID(lid);

    switch (primary) {
    case LANG_RUSSIAN:
    case LANG_BELARUSIAN:
        return LANG_IDX_RU;
    case LANG_UKRAINIAN:
        return LANG_IDX_UK;
    case LANG_CHINESE:
        /* traditional: TW, HK, Macau */
        if (sub == SUBLANG_CHINESE_TRADITIONAL ||
            sub == SUBLANG_CHINESE_HONGKONG ||
            sub == SUBLANG_CHINESE_MACAU)
            return LANG_IDX_ZH_TW;
        return LANG_IDX_ZH_CN;
    case LANG_GERMAN:
        return LANG_IDX_DE;
    case LANG_PORTUGUESE:
        return LANG_IDX_PT;
    default:
        return LANG_IDX_EN;
    }
}

void lang_init(int lcid)
{
    LANGID lid = lcid ? (LANGID)lcid : GetUserDefaultLangID();
    s_lang = lang_from_id(lid);
}

const WCHAR* lang_get(int id)
{
    const WCHAR* str;
    if (id < 0 || id >= STR__COUNT)
        return L"";
    str = s_strings[s_lang][id];
    return str ? str : s_strings[LANG_IDX_EN][id];
}
