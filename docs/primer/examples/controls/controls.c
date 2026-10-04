#include <windows.h>
#include <commctrl.h>

#define FRAME_CLASS L"PrimerControls"
#define IDC_COMMAND_BAR 1
#define IDC_TABS 2
#define IDC_TREE 3
#define IDC_VOLUME 4
#define IDC_LEVEL 5
#define IDC_COPIES 6
#define IDC_COPIES_ARROWS 7
#define IDC_SOUNDS 8
#define IDC_LARGE 9
#define IDC_SMALL 10
#define IDC_APPLY 11
#define IDC_NOTES 12
#define IDC_VOLUME_LABEL 13
#define IDC_LEVEL_LABEL 14
#define IDC_COPIES_LABEL 15
#define TREE_WIDTH 150
#define LABEL_WIDTH 60
#define ROW_HEIGHT 24
#define MARGIN 6
#define PAGE_COUNT 2

static HINSTANCE instance;
static HWND command_bar;
static HWND tabs;
static HWND pages[PAGE_COUNT][16];
static int page_sizes[PAGE_COUNT];

static HWND add_control(int page, DWORD extended, LPCWSTR class_name, LPCWSTR text, DWORD style, int id, HWND parent) {
    HWND control = CreateWindowExW(extended, class_name, text, WS_CHILD | style, 0, 0, 0, 0, parent, (HMENU)id, instance, NULL);
    pages[page][page_sizes[page]++] = control;
    return control;
}

static HTREEITEM add_tree_item(HWND tree, HTREEITEM parent, LPCWSTR text) {
    TVINSERTSTRUCTW insert = {0};
    insert.hParent = parent;
    insert.hInsertAfter = TVI_LAST;
    insert.item.mask = TVIF_TEXT;
    insert.item.pszText = (LPWSTR)text;
    return (HTREEITEM)SendMessageW(tree, TVM_INSERTITEMW, 0, (LPARAM)&insert);
}

static void add_tab(int index, LPCWSTR title) {
    TCITEMW item = {0};
    item.mask = TCIF_TEXT;
    item.pszText = (LPWSTR)title;
    SendMessageW(tabs, TCM_INSERTITEMW, index, (LPARAM)&item);
}

static void show_page(int shown) {
    for (int page = 0; page < PAGE_COUNT; page++) {
        for (int index = 0; index < page_sizes[page]; index++) {
            ShowWindow(pages[page][index], page == shown ? SW_SHOW : SW_HIDE);
        }
    }
}

static void create_settings_page(HWND frame) {
    HWND tree = add_control(0, 0, WC_TREEVIEW, NULL, WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
                            IDC_TREE, frame);
    HTREEITEM device = add_tree_item(tree, TVI_ROOT, L"Velo");
    HTREEITEM sound = add_tree_item(tree, device, L"Sound");
    add_tree_item(tree, device, L"Display");
    add_tree_item(tree, device, L"Power");
    HTREEITEM network = add_tree_item(tree, TVI_ROOT, L"Network");
    add_tree_item(tree, network, L"Modem");
    add_tree_item(tree, network, L"Proxy");
    SendMessageW(tree, TVM_EXPAND, TVE_EXPAND, (LPARAM)device);
    SendMessageW(tree, TVM_EXPAND, TVE_EXPAND, (LPARAM)network);
    SendMessageW(tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)sound);

    add_control(0, 0, L"STATIC", L"Volume:", SS_LEFT, IDC_VOLUME_LABEL, frame);
    HWND volume = add_control(0, 0, TRACKBAR_CLASS, NULL, TBS_HORZ | TBS_AUTOTICKS | WS_TABSTOP, IDC_VOLUME, frame);
    SendMessageW(volume, TBM_SETRANGE, TRUE, MAKELPARAM(0, 10));
    SendMessageW(volume, TBM_SETTICFREQ, 1, 0);
    SendMessageW(volume, TBM_SETPOS, TRUE, 7);

    add_control(0, 0, L"STATIC", L"Level:", SS_LEFT, IDC_LEVEL_LABEL, frame);
    HWND level = add_control(0, 0, PROGRESS_CLASS, NULL, WS_BORDER, IDC_LEVEL, frame);
    SendMessageW(level, PBM_SETRANGE, 0, MAKELPARAM(0, 10));
    SendMessageW(level, PBM_SETPOS, 7, 0);

    add_control(0, 0, L"STATIC", L"Repeats:", SS_LEFT, IDC_COPIES_LABEL, frame);
    HWND copies = add_control(0, 0, L"EDIT", NULL, WS_BORDER | ES_NUMBER | WS_TABSTOP, IDC_COPIES, frame);
    HWND arrows = add_control(0, 0, UPDOWN_CLASS, NULL, UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS, IDC_COPIES_ARROWS, frame);
    SendMessageW(arrows, UDM_SETBUDDY, (WPARAM)copies, 0);
    SendMessageW(arrows, UDM_SETRANGE, 0, MAKELPARAM(9, 1));
    SendMessageW(arrows, UDM_SETPOS, 0, MAKELPARAM(3, 0));

    HWND sounds = add_control(0, 0, L"BUTTON", L"Key clicks", BS_AUTOCHECKBOX | WS_TABSTOP, IDC_SOUNDS, frame);
    SendMessageW(sounds, BM_SETCHECK, BST_CHECKED, 0);
    add_control(0, 0, L"BUTTON", L"Loud", BS_AUTORADIOBUTTON | WS_GROUP | WS_TABSTOP, IDC_LARGE, frame);
    add_control(0, 0, L"BUTTON", L"Soft", BS_AUTORADIOBUTTON, IDC_SMALL, frame);
    CheckRadioButton(frame, IDC_LARGE, IDC_SMALL, IDC_SMALL);
    add_control(0, 0, L"BUTTON", L"Apply", BS_PUSHBUTTON | WS_TABSTOP, IDC_APPLY, frame);
}

static void create_about_page(HWND frame) {
    add_control(1, 0, L"EDIT", L"Each tab is a set of child windows. Selecting a tab hides one set and shows the other.",
                WS_BORDER | ES_MULTILINE | ES_READONLY, IDC_NOTES, frame);
}

static void place(HWND frame, int id, int x, int y, int width, int height) {
    MoveWindow(GetDlgItem(frame, id), x, y, width, height, TRUE);
}

static void layout(HWND frame) {
    RECT client;
    RECT page;
    GetClientRect(frame, &client);
    int top = CommandBar_Height(command_bar);
    MoveWindow(tabs, 0, top, client.right, client.bottom - top, TRUE);
    GetClientRect(tabs, &page);
    SendMessageW(tabs, TCM_ADJUSTRECT, FALSE, (LPARAM)&page);
    page.top += top + MARGIN;
    page.bottom += top - MARGIN;
    page.left += MARGIN;
    page.right -= MARGIN;
    place(frame, IDC_TREE, page.left, page.top, TREE_WIDTH, page.bottom - page.top);
    place(frame, IDC_NOTES, page.left, page.top, page.right - page.left, page.bottom - page.top);
    int label_x = page.left + TREE_WIDTH + MARGIN * 2;
    int x = label_x + LABEL_WIDTH;
    int width = page.right - x;
    int y = page.top;
    place(frame, IDC_VOLUME_LABEL, label_x, y + 4, LABEL_WIDTH, 16);
    place(frame, IDC_VOLUME, x, y, width, ROW_HEIGHT);
    y += ROW_HEIGHT + 6;
    place(frame, IDC_LEVEL_LABEL, label_x, y, LABEL_WIDTH, 16);
    place(frame, IDC_LEVEL, x, y + 1, width, 14);
    y += ROW_HEIGHT;
    place(frame, IDC_COPIES_LABEL, label_x, y + 3, LABEL_WIDTH, 16);
    place(frame, IDC_COPIES, x, y, 50, 20);
    SendMessageW(GetDlgItem(frame, IDC_COPIES_ARROWS), UDM_SETBUDDY, (WPARAM)GetDlgItem(frame, IDC_COPIES), 0);
    y += ROW_HEIGHT + 4;
    place(frame, IDC_SOUNDS, label_x, y, 100, 18);
    place(frame, IDC_LARGE, x + 50, y, 60, 18);
    place(frame, IDC_SMALL, x + 110, y, 60, 18);
    place(frame, IDC_APPLY, page.right - 64, page.bottom - 20, 64, 20);
}

static LRESULT CALLBACK frame_procedure(HWND frame, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_CREATE:
        command_bar = CommandBar_Create(instance, frame, IDC_COMMAND_BAR);
        CommandBar_AddAdornments(command_bar, 0, 0);
        create_settings_page(frame);
        create_about_page(frame);
        tabs = CreateWindowExW(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, frame, (HMENU)IDC_TABS,
                               instance, NULL);
        add_tab(0, L"Settings");
        add_tab(1, L"About");
        show_page(0);
        return 0;
    case WM_SIZE:
        layout(frame);
        return 0;
    case WM_NOTIFY: {
        NMHDR *header = (NMHDR *)lparam;
        if (header->idFrom == IDC_TABS && header->code == TCN_SELCHANGE) {
            show_page((int)SendMessageW(tabs, TCM_GETCURSEL, 0, 0));
        }
        return 0;
    }
    case WM_HSCROLL:
        if ((HWND)lparam == GetDlgItem(frame, IDC_VOLUME)) {
            int volume = (int)SendMessageW((HWND)lparam, TBM_GETPOS, 0, 0);
            SendMessageW(GetDlgItem(frame, IDC_LEVEL), PBM_SETPOS, volume, 0);
        }
        return 0;
    case WM_COMMAND:
        if (LOWORD(wparam) == IDC_APPLY) {
            MessageBeep(MB_OK);
        }
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
    instance = this_instance;
    InitCommonControls();
    window_class.lpfnWndProc = frame_procedure;
    window_class.hInstance = instance;
    window_class.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    window_class.lpszClassName = FRAME_CLASS;
    RegisterClassW(&window_class);
    HWND frame = CreateWindowExW(0, FRAME_CLASS, L"Controls", WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                                 NULL, NULL, instance, NULL);
    if (!frame) {
        return 0;
    }
    ShowWindow(frame, show);
    UpdateWindow(frame);
    while (GetMessageW(&message, NULL, 0, 0)) {
        if (IsDialogMessageW(frame, &message)) {
            continue;
        }
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return message.wParam;
}
