#include <windows.h>
#include <winsock.h>
#include <commctrl.h>

#define FRAME_CLASS L"PrimerFetch"
#define IDC_COMMAND_BAR 1
#define IDC_ADDRESS 2
#define IDC_GO 3
#define IDC_PAGE 4
#define IDC_STATUS 5
#define WM_FETCH_PROGRESS (WM_APP + 1)
#define WM_FETCH_DONE (WM_APP + 2)
#define PROXY_HOST "10.0.2.4"
#define PROXY_PORT 8080
#define ADDRESS_MAX 256
#define RESPONSE_MAX 16384
#define TIMEOUT_MILLISECONDS 30000
#define MARGIN 4
#define ROW_HEIGHT 20
#define GO_WIDTH 40

typedef struct {
    HWND notify;
    char url[ADDRESS_MAX];
    char host[ADDRESS_MAX];
    char data[RESPONSE_MAX];
    int length;
    const WCHAR *error;
} fetch_t;

static HINSTANCE instance;
static HWND command_bar;
static HWND status_bar;
static HWND address;
static HWND go_button;
static HWND page;
static BOOL busy;

static int text_length(const char *text) {
    int length = 0;
    while (text[length]) {
        length++;
    }
    return length;
}

static void host_from_url(const char *url, char *host, int size) {
    const char *start = url;
    for (const char *cursor = url; cursor[0] && cursor[1] && cursor[2]; cursor++) {
        if (cursor[0] == ':' && cursor[1] == '/' && cursor[2] == '/') {
            start = cursor + 3;
            break;
        }
    }
    int length = 0;
    while (start[length] && start[length] != '/' && start[length] != ':' && length < size - 1) {
        host[length] = start[length];
        length++;
    }
    host[length] = 0;
}

static BOOL send_text(SOCKET connection, const char *text) {
    int length = text_length(text);
    while (length > 0) {
        int sent = send(connection, text, length, 0);
        if (sent <= 0) {
            return FALSE;
        }
        text += sent;
        length -= sent;
    }
    return TRUE;
}

static const WCHAR *fetch(fetch_t *request) {
    struct sockaddr_in proxy = {0};
    proxy.sin_family = AF_INET;
    proxy.sin_port = htons(PROXY_PORT);
    proxy.sin_addr.s_addr = inet_addr(PROXY_HOST);
    SOCKET connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connection == INVALID_SOCKET) {
        return L"No sockets. Is the Velo connected?";
    }
    if (connect(connection, (struct sockaddr *)&proxy, sizeof proxy) == SOCKET_ERROR) {
        closesocket(connection);
        return L"Couldn't connect to the proxy.";
    }
    BOOL sent = send_text(connection, "GET ") && send_text(connection, request->url) && send_text(connection, " HTTP/1.0\r\nHost: ") &&
                send_text(connection, request->host) && send_text(connection, "\r\nConnection: close\r\n\r\n");
    const WCHAR *error = sent ? NULL : L"The connection closed while sending.";
    DWORD last_data = GetTickCount();
    while (!error && request->length < RESPONSE_MAX - 1) {
        fd_set readable = {1, {connection}};
        struct timeval interval = {0, 250000};
        int ready = select(0, &readable, NULL, NULL, &interval);
        if (ready == SOCKET_ERROR) {
            error = L"The connection failed.";
        } else if (ready == 0) {
            if (GetTickCount() - last_data > TIMEOUT_MILLISECONDS) {
                error = L"The server stopped answering.";
            }
        } else {
            int got = recv(connection, request->data + request->length, RESPONSE_MAX - 1 - request->length, 0);
            if (got <= 0) {
                break;
            }
            request->length += got;
            last_data = GetTickCount();
            PostMessageW(request->notify, WM_FETCH_PROGRESS, (WPARAM)request->length, 0);
        }
    }
    closesocket(connection);
    request->data[request->length] = 0;
    return error;
}

static DWORD WINAPI fetch_thread(void *parameter) {
    fetch_t *request = parameter;
    request->error = fetch(request);
    if (!PostMessageW(request->notify, WM_FETCH_DONE, 0, (LPARAM)request)) {
        LocalFree(request);
    }
    return 0;
}

static int utf8_to_edit_text(const char *text, int length, WCHAR *output, int size) {
    int written = 0;
    for (int index = 0; index < length && written < size - 2;) {
        unsigned char byte = (unsigned char)text[index];
        DWORD code = '?';
        int extra = byte >= 0xF0 ? 3 : byte >= 0xE0 ? 2 : byte >= 0xC0 ? 1 : 0;
        if (byte < 0x80) {
            code = byte;
        } else if (extra && index + extra < length) {
            code = byte & (0x3F >> extra);
            for (int follow = 1; follow <= extra; follow++) {
                code = (code << 6) | (text[index + follow] & 0x3F);
            }
        }
        index += extra + 1;
        if (code == '\n' && (written == 0 || output[written - 1] != '\r')) {
            output[written++] = '\r';
        }
        output[written++] = code > 0xFFFF ? '?' : (WCHAR)code;
    }
    output[written] = 0;
    return written;
}

static void set_status(const WCHAR *text) {
    SendMessageW(status_bar, SB_SETTEXT, 0, (LPARAM)text);
}

static void start_fetch(HWND frame) {
    WCHAR wide_url[ADDRESS_MAX];
    if (busy) {
        return;
    }
    fetch_t *request = LocalAlloc(LPTR, sizeof(fetch_t));
    if (!request) {
        set_status(L"Not enough memory.");
        return;
    }
    GetWindowTextW(address, wide_url, ADDRESS_MAX);
    for (int index = 0; index < ADDRESS_MAX; index++) {
        request->url[index] = wide_url[index] < 0x80 ? (char)wide_url[index] : '?';
        if (!wide_url[index]) {
            break;
        }
    }
    host_from_url(request->url, request->host, ADDRESS_MAX);
    request->notify = frame;
    HANDLE thread = CreateThread(NULL, 0, fetch_thread, request, 0, NULL);
    if (!thread) {
        LocalFree(request);
        set_status(L"Couldn't start a thread.");
        return;
    }
    CloseHandle(thread);
    busy = TRUE;
    EnableWindow(go_button, FALSE);
    SetWindowTextW(page, L"");
    set_status(L"Connecting...");
}

static void fetch_done(fetch_t *request) {
    WCHAR *text = LocalAlloc(LPTR, (RESPONSE_MAX * 2 + 1) * sizeof(WCHAR));
    if (text) {
        utf8_to_edit_text(request->data, request->length, text, RESPONSE_MAX * 2 + 1);
        SetWindowTextW(page, text);
        LocalFree(text);
    }
    if (request->error) {
        set_status(request->error);
    } else {
        WCHAR status[64];
        wsprintfW(status, L"Done, %d bytes", request->length);
        set_status(status);
    }
    LocalFree(request);
    busy = FALSE;
    EnableWindow(go_button, TRUE);
}

static void layout(HWND frame) {
    RECT client;
    RECT status;
    GetClientRect(frame, &client);
    SendMessageW(status_bar, WM_SIZE, 0, 0);
    GetWindowRect(status_bar, &status);
    int top = CommandBar_Height(command_bar) + MARGIN;
    int bottom = client.bottom - (status.bottom - status.top);
    MoveWindow(address, MARGIN, top, client.right - GO_WIDTH - MARGIN * 3, ROW_HEIGHT, TRUE);
    MoveWindow(go_button, client.right - GO_WIDTH - MARGIN, top, GO_WIDTH, ROW_HEIGHT, TRUE);
    top += ROW_HEIGHT + MARGIN;
    MoveWindow(page, 0, top, client.right, bottom - top, TRUE);
}

static LRESULT CALLBACK frame_procedure(HWND frame, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_CREATE:
        command_bar = CommandBar_Create(instance, frame, IDC_COMMAND_BAR);
        CommandBar_AddAdornments(command_bar, 0, 0);
        status_bar = CreateStatusWindowW(WS_CHILD | WS_VISIBLE, L"Ready", frame, IDC_STATUS);
        address = CreateWindowExW(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP, 0, 0, 0, 0, frame,
                                  (HMENU)IDC_ADDRESS, instance, NULL);
        go_button = CreateWindowExW(0, L"BUTTON", L"Go", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP, 0, 0, 0, 0, frame,
                                    (HMENU)IDC_GO, instance, NULL);
        page = CreateWindowExW(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 0, 0, 0, 0,
                               frame, (HMENU)IDC_PAGE, instance, NULL);
        SendMessageW(page, EM_LIMITTEXT, RESPONSE_MAX * 2, 0);
        return 0;
    case WM_SIZE:
        layout(frame);
        return 0;
    case WM_COMMAND:
        if (LOWORD(wparam) == IDC_GO) {
            start_fetch(frame);
        }
        return 0;
    case WM_FETCH_PROGRESS: {
        WCHAR status[64];
        wsprintfW(status, L"Received %d bytes", (int)wparam);
        set_status(status);
        return 0;
    }
    case WM_FETCH_DONE:
        fetch_done((fetch_t *)lparam);
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
    HWND frame = CreateWindowExW(0, FRAME_CLASS, L"Fetch", WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL,
                                 instance, NULL);
    if (!frame) {
        return 0;
    }
    ShowWindow(frame, show);
    UpdateWindow(frame);
    SetWindowTextW(address, command_line[0] ? command_line : L"http://example.com/");
    if (command_line[0]) {
        start_fetch(frame);
    }
    SetFocus(address);
    while (GetMessageW(&message, NULL, 0, 0)) {
        if (IsDialogMessageW(frame, &message)) {
            continue;
        }
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return message.wParam;
}
