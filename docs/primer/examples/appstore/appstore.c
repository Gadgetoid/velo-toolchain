#include <windows.h>
#include <commctrl.h>

#define FRAME_CLASS L"PrimerAppStore"
#define IDC_COMMAND_BAR 1
#define IDC_APPS 2
#define IDC_INSTALL 3
#define IDC_PROGRESS 4
#define IDC_STATUS 5
#define LIST_PERCENT 55
#define MARGIN 6
#define BUTTON_WIDTH 64
#define BUTTON_HEIGHT 20
#define PROGRESS_HEIGHT 12
#define DOWNLOAD_TIMER 1
#define APP_COUNT (sizeof apps / sizeof apps[0])

typedef struct {
    const WCHAR *name;
    const WCHAR *version;
    int size_kb;
    const WCHAR *author;
    const WCHAR *description;
    BOOL installed;
} app_t;

static app_t apps[] = {
    {L"Bluesky", L"1.0", 96, L"velo-bluesky", L"Read your timeline and threads, post, reply and like, through a web proxy.", FALSE},
    {L"Pocket Clock", L"2.1", 18, L"Example Software", L"A big clock with alarms and a world map.", TRUE},
    {L"Mines", L"1.3", 24, L"Example Software", L"The classic, with pen-friendly flagging.", FALSE},
    {L"Units", L"0.9", 12, L"Example Software", L"Converts between metric and imperial units.", FALSE},
    {L"Terminal", L"1.1", 40, L"Example Software", L"A serial terminal for the Velo's PC Link port.", TRUE},
};

static HINSTANCE instance;
static HWND command_bar;
static HWND status_bar;
static HWND app_list;
static HWND install_button;
static HWND progress_bar;
static HFONT title_font;
static int selected = -1;
static int downloading = -1;
static int download_percent;
static RECT details;

static void add_column(int index, const WCHAR *title, int width, int format) {
    LVCOLUMNW column = {0};
    column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT | LVCF_SUBITEM;
    column.fmt = format;
    column.cx = width;
    column.pszText = (LPWSTR)title;
    column.iSubItem = index;
    SendMessageW(app_list, LVM_INSERTCOLUMNW, index, (LPARAM)&column);
}

static void set_cell(int row, int column, const WCHAR *text) {
    LVITEMW item = {0};
    item.iSubItem = column;
    item.pszText = (LPWSTR)text;
    SendMessageW(app_list, LVM_SETITEMTEXTW, row, (LPARAM)&item);
}

static void fill_list(void) {
    HIMAGELIST icons = ImageList_Create(16, 16, ILC_COLOR | ILC_MASK, 1, 0);
    ImageList_AddIcon(icons, LoadImageW(instance, MAKEINTRESOURCEW(1), IMAGE_ICON, 16, 16, 0));
    SendMessageW(app_list, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)icons);
    SendMessageW(app_list, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
    add_column(0, L"Name", 100, LVCFMT_LEFT);
    add_column(1, L"Version", 50, LVCFMT_LEFT);
    add_column(2, L"Size", 50, LVCFMT_RIGHT);
    for (int index = 0; index < (int)APP_COUNT; index++) {
        WCHAR size[16];
        LVITEMW item = {0};
        item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
        item.iItem = index;
        item.pszText = (LPWSTR)apps[index].name;
        item.iImage = 0;
        item.lParam = index;
        SendMessageW(app_list, LVM_INSERTITEMW, 0, (LPARAM)&item);
        wsprintfW(size, L"%d KB", apps[index].size_kb);
        set_cell(index, 1, apps[index].version);
        set_cell(index, 2, size);
    }
}

static void select_row(int row) {
    LVITEMW item = {0};
    item.state = LVIS_SELECTED | LVIS_FOCUSED;
    item.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
    SendMessageW(app_list, LVM_SETITEMSTATE, row, (LPARAM)&item);
}

static void update_status(void) {
    STORE_INFORMATION store;
    WCHAR text[64];
    int installed = 0;
    for (int index = 0; index < (int)APP_COUNT; index++) {
        installed += apps[index].installed;
    }
    wsprintfW(text, L"%d apps, %d installed", (int)APP_COUNT, installed);
    SendMessageW(status_bar, SB_SETTEXT, 0, (LPARAM)text);
    if (GetStoreInformation(&store)) {
        wsprintfW(text, L"%d KB free", (int)(store.dwFreeSize / 1024));
        SendMessageW(status_bar, SB_SETTEXT, 1, (LPARAM)text);
    }
}

static void update_details(void) {
    BOOL busy = downloading >= 0;
    BOOL can_install = selected >= 0 && !apps[selected].installed && !busy;
    SetWindowTextW(install_button, selected >= 0 && apps[selected].installed ? L"Installed" : L"Install");
    EnableWindow(install_button, can_install);
    ShowWindow(progress_bar, busy && downloading == selected ? SW_SHOW : SW_HIDE);
    InvalidateRect(GetParent(app_list), &details, TRUE);
}

static void start_download(HWND frame) {
    downloading = selected;
    download_percent = 0;
    SendMessageW(progress_bar, PBM_SETPOS, 0, 0);
    SetTimer(frame, DOWNLOAD_TIMER, 1000, NULL);
    update_details();
}

static void download_step(HWND frame) {
    download_percent++;
    SendMessageW(progress_bar, PBM_SETPOS, download_percent, 0);
    if (download_percent >= 100) {
        KillTimer(frame, DOWNLOAD_TIMER);
        apps[downloading].installed = TRUE;
        downloading = -1;
        update_status();
        update_details();
    }
}

static void paint_details(HWND frame) {
    PAINTSTRUCT paint;
    WCHAR line[64];
    HDC dc = BeginPaint(frame, &paint);
    if (selected >= 0) {
        const app_t *app = &apps[selected];
        RECT area = details;
        SetBkMode(dc, TRANSPARENT);
        HGDIOBJ previous = SelectObject(dc, title_font);
        DrawTextW(dc, app->name, -1, &area, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
        SelectObject(dc, previous);
        area.top += 22;
        wsprintfW(line, L"Version %s by %s", app->version, app->author);
        DrawTextW(dc, line, -1, &area, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);
        area.top += 20;
        DrawTextW(dc, app->description, -1, &area, DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
    }
    EndPaint(frame, &paint);
}

static void layout(HWND frame) {
    RECT client;
    RECT status;
    GetClientRect(frame, &client);
    SendMessageW(status_bar, WM_SIZE, 0, 0);
    GetWindowRect(status_bar, &status);
    int top = CommandBar_Height(command_bar);
    int bottom = client.bottom - (status.bottom - status.top);
    int split = client.right * LIST_PERCENT / 100;
    MoveWindow(app_list, 0, top, split, bottom - top, TRUE);
    SetRect(&details, split + MARGIN, top + MARGIN, client.right - MARGIN, bottom - BUTTON_HEIGHT - MARGIN * 2);
    MoveWindow(install_button, client.right - MARGIN - BUTTON_WIDTH, bottom - MARGIN - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
    MoveWindow(progress_bar, split + MARGIN, bottom - MARGIN - (BUTTON_HEIGHT + PROGRESS_HEIGHT) / 2,
               client.right - split - BUTTON_WIDTH - MARGIN * 3, PROGRESS_HEIGHT, TRUE);
    InvalidateRect(frame, NULL, TRUE);
}

static void create_children(HWND frame) {
    int edges[] = {300, -1};
    LOGFONTW font = {0};
    command_bar = CommandBar_Create(instance, frame, IDC_COMMAND_BAR);
    CommandBar_AddAdornments(command_bar, 0, 0);
    status_bar = CreateStatusWindowW(WS_CHILD | WS_VISIBLE, L"", frame, IDC_STATUS);
    SendMessageW(status_bar, SB_SETPARTS, 2, (LPARAM)edges);
    app_list = CreateWindowExW(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                               0, 0, 0, 0, frame, (HMENU)IDC_APPS, instance, NULL);
    install_button = CreateWindowExW(0, L"BUTTON", L"Install", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, frame,
                                     (HMENU)IDC_INSTALL, instance, NULL);
    progress_bar = CreateWindowExW(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_BORDER, 0, 0, 0, 0, frame, (HMENU)IDC_PROGRESS, instance, NULL);
    SendMessageW(progress_bar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    GetObjectW(GetStockObject(SYSTEM_FONT), sizeof font, &font);
    font.lfWeight = FW_BOLD;
    font.lfHeight = font.lfHeight * 3 / 2;
    font.lfWidth = 0;
    title_font = CreateFontIndirectW(&font);
    fill_list();
}

static LRESULT CALLBACK frame_procedure(HWND frame, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_CREATE:
        create_children(frame);
        return 0;
    case WM_SIZE:
        layout(frame);
        return 0;
    case WM_PAINT:
        paint_details(frame);
        return 0;
    case WM_NOTIFY: {
        NMLISTVIEW *change = (NMLISTVIEW *)lparam;
        if (change->hdr.idFrom == IDC_APPS && change->hdr.code == LVN_ITEMCHANGED && (change->uNewState & LVIS_SELECTED)) {
            selected = (int)change->lParam;
            update_details();
        }
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(wparam) == IDC_INSTALL && selected >= 0) {
            start_download(frame);
        }
        return 0;
    case WM_TIMER:
        if (wparam == DOWNLOAD_TIMER) {
            download_step(frame);
        }
        return 0;
    case WM_CLOSE:
        DestroyWindow(frame);
        return 0;
    case WM_DESTROY:
        CommandBar_Destroy(command_bar);
        DeleteObject(title_font);
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
    window_class.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    window_class.lpszClassName = FRAME_CLASS;
    RegisterClassW(&window_class);
    HWND frame = CreateWindowExW(0, FRAME_CLASS, L"App Store", WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
                                 NULL, NULL, instance, NULL);
    if (!frame) {
        return 0;
    }
    SendMessageW(frame, WM_SETICON, ICON_SMALL, (LPARAM)LoadImageW(instance, MAKEINTRESOURCEW(1), IMAGE_ICON, 16, 16, 0));
    ShowWindow(frame, show);
    UpdateWindow(frame);
    update_status();
    select_row(0);
    start_download(frame);
    SetFocus(app_list);
    while (GetMessageW(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return message.wParam;
}
