#include <windows.h>

#include "resource.h"

#define SETTINGS_KEY L"Software\\Velo Primer\\Form"
#define HANDLE_MAX 64

typedef struct {
    WCHAR handle[HANDLE_MAX];
    DWORD server;
    DWORD refresh;
    DWORD pictures;
} account_t;

static const WCHAR *servers[] = {L"bsky.social", L"Your own PDS", L"Test server"};

static void read_number(HKEY key, LPCWSTR name, DWORD *value) {
    DWORD type;
    DWORD size = sizeof *value;
    DWORD stored;
    if (RegQueryValueExW(key, name, NULL, &type, (BYTE *)&stored, &size) == ERROR_SUCCESS && type == REG_DWORD) {
        *value = stored;
    }
}

static void load_account(account_t *account) {
    HKEY key;
    DWORD type;
    DWORD size = sizeof account->handle;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, SETTINGS_KEY, 0, KEY_READ, &key) != ERROR_SUCCESS) {
        return;
    }
    if (RegQueryValueExW(key, L"Handle", NULL, &type, (BYTE *)account->handle, &size) != ERROR_SUCCESS || type != REG_SZ) {
        account->handle[0] = 0;
    }
    account->handle[HANDLE_MAX - 1] = 0;
    read_number(key, L"Server", &account->server);
    read_number(key, L"Refresh", &account->refresh);
    read_number(key, L"Pictures", &account->pictures);
    RegCloseKey(key);
}

static void save_account(const account_t *account) {
    HKEY key;
    DWORD disposition;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, SETTINGS_KEY, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, &disposition) != ERROR_SUCCESS) {
        return;
    }
    RegSetValueExW(key, L"Handle", 0, REG_SZ, (const BYTE *)account->handle, (wcslen(account->handle) + 1) * sizeof(WCHAR));
    RegSetValueExW(key, L"Server", 0, REG_DWORD, (const BYTE *)&account->server, sizeof(DWORD));
    RegSetValueExW(key, L"Refresh", 0, REG_DWORD, (const BYTE *)&account->refresh, sizeof(DWORD));
    RegSetValueExW(key, L"Pictures", 0, REG_DWORD, (const BYTE *)&account->pictures, sizeof(DWORD));
    RegCloseKey(key);
}

static void fill_dialog(HWND dialog, const account_t *account) {
    HWND server = GetDlgItem(dialog, IDC_SERVER);
    SetWindowTextW(GetDlgItem(dialog, IDC_HANDLE), account->handle);
    SendMessageW(GetDlgItem(dialog, IDC_HANDLE), EM_LIMITTEXT, HANDLE_MAX - 1, 0);
    for (int index = 0; index < 3; index++) {
        SendMessageW(server, CB_ADDSTRING, 0, (LPARAM)servers[index]);
    }
    SendMessageW(server, CB_SETCURSEL, account->server, 0);
    CheckRadioButton(dialog, IDC_MANUAL, IDC_DAILY, IDC_MANUAL + account->refresh);
    SendMessageW(GetDlgItem(dialog, IDC_PICTURES), BM_SETCHECK, account->pictures ? BST_CHECKED : BST_UNCHECKED, 0);
}

static BOOL read_dialog(HWND dialog, account_t *account) {
    GetWindowTextW(GetDlgItem(dialog, IDC_HANDLE), account->handle, HANDLE_MAX);
    if (!account->handle[0]) {
        MessageBoxW(dialog, L"Enter your handle.", L"Account", MB_OK | MB_ICONEXCLAMATION);
        SetFocus(GetDlgItem(dialog, IDC_HANDLE));
        return FALSE;
    }
    account->server = (DWORD)SendMessageW(GetDlgItem(dialog, IDC_SERVER), CB_GETCURSEL, 0, 0);
    for (int id = IDC_MANUAL; id <= IDC_DAILY; id++) {
        if (SendMessageW(GetDlgItem(dialog, id), BM_GETCHECK, 0, 0) == BST_CHECKED) {
            account->refresh = (DWORD)(id - IDC_MANUAL);
        }
    }
    account->pictures = SendMessageW(GetDlgItem(dialog, IDC_PICTURES), BM_GETCHECK, 0, 0) == BST_CHECKED;
    return TRUE;
}

static BOOL CALLBACK account_procedure(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
    account_t *account = (account_t *)GetWindowLongW(dialog, DWL_USER);
    switch (message) {
    case WM_INITDIALOG:
        account = (account_t *)lparam;
        SetWindowLongW(dialog, DWL_USER, (LONG)account);
        fill_dialog(dialog, account);
        SetFocus(GetDlgItem(dialog, IDC_PASSWORD));
        return FALSE;
    case WM_COMMAND:
        if (LOWORD(wparam) == IDOK) {
            if (read_dialog(dialog, account)) {
                EndDialog(dialog, IDOK);
            }
            return TRUE;
        }
        if (LOWORD(wparam) == IDCANCEL) {
            EndDialog(dialog, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static int run_dialog(HINSTANCE instance, int id, HWND owner, DLGPROC procedure, void *parameter) {
    HRSRC resource = FindResource(instance, MAKEINTRESOURCEW(id), RT_DIALOG);
    HGLOBAL loaded = resource ? LoadResource(instance, resource) : NULL;
    if (!loaded) {
        return -1;
    }
    return DialogBoxIndirectParamW(instance, LockResource(loaded), owner, procedure, (LPARAM)parameter);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPWSTR command_line, int show) {
    account_t account = {L"velo.example.com", 0, 1, 1};
    WCHAR summary[160];
    load_account(&account);
    if (run_dialog(instance, IDD_ACCOUNT, NULL, account_procedure, &account) != IDOK) {
        return 0;
    }
    save_account(&account);
    wsprintfW(summary, L"Saved %s on %s.", account.handle, servers[account.server]);
    MessageBoxW(NULL, summary, L"Account", MB_OK | MB_ICONINFORMATION);
    return 0;
}
