#include <windows.h>
#include <commctrl.h>

#include "resource.h"

#define FRAME_CLASS L"PrimerFrame"
#define LIST_PERCENT 38
#define FOLDER_WIDTH 90
#define NOTE_COUNT (sizeof notes / sizeof notes[0])

typedef struct {
    const WCHAR *subject;
    const WCHAR *body;
} note_t;

static const note_t notes[] = {
    {L"Welcome to the Velo", L"This is a two-pane layout: a list on the left, the selected item on the right, "
                             L"a command bar on top and a status bar underneath.\r\n\r\nTap a note to read it."},
    {L"Shopping", L"Batteries (AA x2)\r\nPC Card, 8 MB\r\nSerial cable"},
    {L"Meeting notes", L"Ship the CE 2.0 build on Friday. Check the help file still fits on the card."},
    {L"Ideas", L"A Bluesky client.\r\nAn app store that installs from the web.\r\nA better clock."},
    {L"Phone numbers", L"Office: 555 0100\r\nHome: 555 0199"},
};

static HINSTANCE instance;
static HWND command_bar;
static HWND status_bar;
static HWND note_list;
static HWND reader;

static void create_command_bar(HWND frame) {
    static const int commands[] = {ID_FILE_NEW, ID_FILE_DELETE};
    static const int bitmaps[] = {STD_FILENEW, STD_DELETE};
    TBBUTTON buttons[3] = {0};
    command_bar = CommandBar_Create(instance, frame, IDC_COMMAND_BAR);
    CommandBar_InsertMenubar(command_bar, instance, IDM_MAIN, 0);
    CommandBar_AddBitmap(command_bar, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, 15, 16, 16);
    buttons[0].fsStyle = TBSTYLE_SEP;
    for (int index = 0; index < 2; index++) {
        buttons[index + 1].iBitmap = bitmaps[index];
        buttons[index + 1].idCommand = commands[index];
        buttons[index + 1].fsState = TBSTATE_ENABLED;
        buttons[index + 1].fsStyle = TBSTYLE_BUTTON;
    }
    CommandBar_AddButtons(command_bar, 3, buttons);
    HWND folder = CommandBar_InsertComboBox(command_bar, instance, FOLDER_WIDTH, CBS_DROPDOWNLIST | WS_VSCROLL, IDC_FOLDER, 4);
    SendMessageW(folder, CB_ADDSTRING, 0, (LPARAM)L"All Notes");
    SendMessageW(folder, CB_ADDSTRING, 0, (LPARAM)L"Personal");
    SendMessageW(folder, CB_ADDSTRING, 0, (LPARAM)L"Work");
    SendMessageW(folder, CB_SETCURSEL, 0, 0);
    CommandBar_AddAdornments(command_bar, CMDBAR_HELP, 0);
}

static void create_status_bar(HWND frame) {
    int edges[] = {300, -1};
    status_bar = CreateStatusWindowW(WS_CHILD | WS_VISIBLE, L"", frame, IDC_STATUS);
    SendMessageW(status_bar, SB_SETPARTS, 2, (LPARAM)edges);
    SendMessageW(status_bar, SB_SETTEXT, 1, (LPARAM)L"Offline");
}

static void show_note(int index) {
    WCHAR status[64];
    if (index < 0 || index >= (int)NOTE_COUNT) {
        return;
    }
    SendMessageW(note_list, LB_SETCURSEL, index, 0);
    SetWindowTextW(reader, notes[index].body);
    wsprintfW(status, L"Note %d of %d", index + 1, (int)NOTE_COUNT);
    SendMessageW(status_bar, SB_SETTEXT, 0, (LPARAM)status);
}

static void layout(HWND frame) {
    RECT client;
    RECT status;
    GetClientRect(frame, &client);
    SendMessageW(status_bar, WM_SIZE, 0, 0);
    GetWindowRect(status_bar, &status);
    int top = CommandBar_Height(command_bar);
    int height = client.bottom - top - (status.bottom - status.top);
    int split = client.right * LIST_PERCENT / 100;
    MoveWindow(note_list, 0, top, split, height, TRUE);
    MoveWindow(reader, split, top, client.right - split, height, TRUE);
}

static void command(HWND frame, int id) {
    int selected = (int)SendMessageW(note_list, LB_GETCURSEL, 0, 0);
    switch (id) {
    case ID_VIEW_NEXT:
        show_note(selected + 1);
        break;
    case ID_VIEW_PREVIOUS:
        show_note(selected - 1);
        break;
    case ID_FILE_NEW:
    case ID_FILE_DELETE:
        MessageBoxW(frame, L"Not in this example.", L"Notes", MB_OK | MB_ICONINFORMATION);
        break;
    case ID_HELP_ABOUT:
        MessageBoxW(frame, L"Notes 1.0\nA velo-toolchain primer example.", L"About Notes", MB_OK | MB_ICONINFORMATION);
        break;
    case ID_FILE_EXIT:
        DestroyWindow(frame);
        break;
    }
}

static LRESULT CALLBACK frame_procedure(HWND frame, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_CREATE:
        create_command_bar(frame);
        create_status_bar(frame);
        note_list = CreateWindowExW(0, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
                                    0, 0, 0, 0, frame, (HMENU)IDC_NOTES, instance, NULL);
        reader = CreateWindowExW(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
                                 0, 0, 0, 0, frame, (HMENU)IDC_READER, instance, NULL);
        for (int index = 0; index < (int)NOTE_COUNT; index++) {
            SendMessageW(note_list, LB_ADDSTRING, 0, (LPARAM)notes[index].subject);
        }
        return 0;
    case WM_SIZE:
        layout(frame);
        return 0;
    case WM_ACTIVATE:
        if (LOWORD(wparam)) {
            SetFocus(note_list);
        }
        return 0;
    case WM_COMMAND:
        if (LOWORD(wparam) == IDC_NOTES && HIWORD(wparam) == LBN_SELCHANGE) {
            show_note((int)SendMessageW(note_list, LB_GETCURSEL, 0, 0));
        } else {
            command(frame, LOWORD(wparam));
        }
        return 0;
    case WM_HELP:
        command(frame, ID_HELP_ABOUT);
        return 0;
    case WM_CLOSE:
        DestroyWindow(frame);
        return 0;
    case WM_DESTROY:
        CommandBar_Destroy(command_bar);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(frame, message, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE this_instance, HINSTANCE previous, LPWSTR command_line, int show) {
    WNDCLASSW window_class = {0};
    MSG message;
    HWND existing = FindWindowW(FRAME_CLASS, NULL);
    if (existing) {
        SetForegroundWindow(existing);
        return 0;
    }
    instance = this_instance;
    InitCommonControls();
    window_class.lpfnWndProc = frame_procedure;
    window_class.hInstance = instance;
    window_class.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    window_class.lpszClassName = FRAME_CLASS;
    RegisterClassW(&window_class);
    HWND frame = CreateWindowExW(0, FRAME_CLASS, L"Notes", WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                                 NULL, NULL, instance, NULL);
    if (!frame) {
        return 0;
    }
    ShowWindow(frame, show);
    UpdateWindow(frame);
    show_note(0);
    HACCEL accelerators = LoadAcceleratorsW(instance, MAKEINTRESOURCEW(IDA_MAIN));
    while (GetMessageW(&message, NULL, 0, 0)) {
        if (TranslateAcceleratorW(frame, accelerators, &message)) {
            continue;
        }
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return message.wParam;
}
