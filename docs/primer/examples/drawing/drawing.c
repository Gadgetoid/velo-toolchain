#include <windows.h>
#include <commctrl.h>

#define FRAME_CLASS L"PrimerDrawing"
#define IDC_COMMAND_BAR 1
#define GREY_STEPS 16
#define STEP_WIDTH 16
#define SWATCH_SIZE 24
#define MARGIN 6

typedef struct {
    HFONT normal;
    HFONT bold;
    HFONT large;
    HFONT italic;
    HFONT fixed;
} fonts_t;

static HINSTANCE instance;
static HWND command_bar;
static fonts_t fonts;

static HFONT make_font(const LOGFONTW *base, LONG weight, LONG height, BYTE italic, const WCHAR *face) {
    LOGFONTW font = *base;
    font.lfWeight = weight;
    font.lfHeight = height;
    font.lfWidth = 0;
    font.lfItalic = italic;
    if (face) {
        int length = 0;
        while (face[length] && length < 31) {
            font.lfFaceName[length] = face[length];
            length++;
        }
        font.lfFaceName[length] = 0;
        font.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
    }
    return CreateFontIndirectW(&font);
}

static void create_fonts(void) {
    LOGFONTW base = {0};
    GetObjectW(GetStockObject(SYSTEM_FONT), sizeof base, &base);
    fonts.normal = make_font(&base, FW_NORMAL, base.lfHeight, 0, NULL);
    fonts.bold = make_font(&base, FW_BOLD, base.lfHeight, 0, NULL);
    fonts.large = make_font(&base, FW_BOLD, base.lfHeight * 2, 0, NULL);
    fonts.italic = make_font(&base, FW_NORMAL, base.lfHeight, 1, NULL);
    fonts.fixed = make_font(&base, FW_NORMAL, base.lfHeight, 0, L"Courier New");
}

static void destroy_fonts(void) {
    DeleteObject(fonts.normal);
    DeleteObject(fonts.bold);
    DeleteObject(fonts.large);
    DeleteObject(fonts.italic);
    DeleteObject(fonts.fixed);
}

static int text_line(HDC dc, HFONT font, int x, int y, const WCHAR *text) {
    TEXTMETRICW metrics;
    HGDIOBJ previous = SelectObject(dc, font);
    ExtTextOutW(dc, x, y, 0, NULL, text, wcslen(text), NULL);
    GetTextMetricsW(dc, &metrics);
    SelectObject(dc, previous);
    return y + metrics.tmHeight + metrics.tmExternalLeading;
}

static void fill(HDC dc, int left, int top, int right, int bottom, COLORREF colour) {
    RECT area = {left, top, right, bottom};
    HBRUSH brush = CreateSolidBrush(colour);
    FillRect(dc, &area, brush);
    DeleteObject(brush);
}

static int draw_greys(HDC dc, int x, int y) {
    static const int safe[] = {0, 85, 170, 255};
    WCHAR label[8];
    y = text_line(dc, fonts.normal, x, y, L"RGB(17n, 17n, 17n):");
    for (int step = 0; step < GREY_STEPS; step++) {
        int level = step * 17;
        fill(dc, x + step * STEP_WIDTH, y, x + (step + 1) * STEP_WIDTH, y + SWATCH_SIZE / 2, RGB(level, level, level));
    }
    y = text_line(dc, fonts.normal, x, y + SWATCH_SIZE / 2 + 2, L"Safe on CE 1.0 and 2.0:");
    for (int index = 0; index < 4; index++) {
        int left = x + index * (SWATCH_SIZE + 40);
        fill(dc, left, y, left + SWATCH_SIZE, y + SWATCH_SIZE, RGB(safe[index], safe[index], safe[index]));
        Rectangle(dc, left - 1, y - 1, left + SWATCH_SIZE + 1, y + SWATCH_SIZE + 1);
        fill(dc, left, y, left + SWATCH_SIZE, y + SWATCH_SIZE, RGB(safe[index], safe[index], safe[index]));
        wsprintfW(label, L"%d", safe[index]);
        text_line(dc, fonts.normal, left + SWATCH_SIZE + 4, y + 4, label);
    }
    return y + SWATCH_SIZE;
}

static void draw_shapes(HDC dc, int x, int y) {
    LOGPEN dashed = {PS_DASH, {1, 0}, RGB(0, 0, 0)};
    RECT edge = {x, y, x + 40, y + 28};
    POINT zigzag[] = {{x + 150, y + 28}, {x + 162, y}, {x + 174, y + 28}, {x + 186, y}, {x + 198, y + 28}};
    SelectObject(dc, GetStockObject(WHITE_BRUSH));
    DrawEdge(dc, &edge, EDGE_RAISED, BF_RECT);
    SetRect(&edge, x + 48, y, x + 88, y + 28);
    DrawEdge(dc, &edge, EDGE_SUNKEN, BF_RECT);
    SetRect(&edge, x + 96, y, x + 136, y + 28);
    DrawEdge(dc, &edge, EDGE_ETCHED, BF_RECT);
    Polyline(dc, zigzag, 5);
    HPEN pen = CreatePenIndirect(&dashed);
    HGDIOBJ previous = SelectObject(dc, pen);
    Ellipse(dc, x + 206, y, x + 246, y + 28);
    SelectObject(dc, previous);
    DeleteObject(pen);
    SelectObject(dc, GetStockObject(GRAY_BRUSH));
    Rectangle(dc, x + 254, y, x + 294, y + 28);
    PatBlt(dc, x + 264, y + 7, 20, 14, DSTINVERT);
}

static void draw_card(HDC dc, const RECT *card) {
    RECT text = *card;
    SelectObject(dc, GetStockObject(WHITE_BRUSH));
    Rectangle(dc, card->left, card->top, card->right, card->bottom);
    SelectObject(dc, GetStockObject(GRAY_BRUSH));
    Ellipse(dc, card->left + 4, card->top + 4, card->left + 28, card->top + 28);
    int y = text_line(dc, fonts.bold, card->left + 34, card->top + 3, L"Velo Fan");
    SetTextColor(dc, RGB(85, 85, 85));
    text_line(dc, fonts.normal, card->left + 34, y, L"@velo.example.com  2h");
    SetTextColor(dc, RGB(0, 0, 0));
    text.left += 34;
    text.right -= 4;
    text.top += 32;
    SelectObject(dc, fonts.normal);
    DrawTextW(dc, L"Wrapped text is DrawTextW with DT_WORDBREAK. It breaks at spaces and clips to the rectangle.", -1, &text,
              DT_LEFT | DT_WORDBREAK | DT_NOPREFIX);
}

static void paint(HWND frame, HDC dc, const RECT *client) {
    WCHAR depth[64];
    int top = CommandBar_Height(command_bar) + MARGIN;
    FillRect(dc, client, GetStockObject(WHITE_BRUSH));
    SetBkMode(dc, TRANSPARENT);
    int y = draw_greys(dc, MARGIN, top);
    draw_shapes(dc, MARGIN, y + MARGIN * 2);
    int x = MARGIN * 2 + 300;
    int text_y = text_line(dc, fonts.large, x, top, L"Large bold");
    text_y = text_line(dc, fonts.bold, x, text_y, L"System font, bold");
    text_y = text_line(dc, fonts.normal, x, text_y, L"System font, normal");
    text_y = text_line(dc, fonts.italic, x, text_y, L"System font, italic");
    text_y = text_line(dc, fonts.fixed, x, text_y, L"Courier New 0O1lI");
    wsprintfW(depth, L"%d bits per pixel", GetDeviceCaps(dc, BITSPIXEL));
    text_line(dc, fonts.normal, x, text_y, depth);
    RECT card = {MARGIN, y + 28 + MARGIN * 4, MARGIN + 300, client->bottom - MARGIN};
    draw_card(dc, &card);
}

static void paint_buffered(HWND frame) {
    PAINTSTRUCT paint_info;
    RECT client;
    HDC dc = BeginPaint(frame, &paint_info);
    GetClientRect(frame, &client);
    HDC buffer = CreateCompatibleDC(dc);
    HBITMAP bitmap = CreateCompatibleBitmap(dc, client.right, client.bottom);
    HGDIOBJ previous_bitmap = SelectObject(buffer, bitmap);
    HGDIOBJ previous_font = SelectObject(buffer, fonts.normal);
    HGDIOBJ previous_brush = SelectObject(buffer, GetStockObject(WHITE_BRUSH));
    paint(frame, buffer, &client);
    BitBlt(dc, 0, 0, client.right, client.bottom, buffer, 0, 0, SRCCOPY);
    SelectObject(buffer, previous_brush);
    SelectObject(buffer, previous_font);
    SelectObject(buffer, previous_bitmap);
    DeleteObject(bitmap);
    DeleteDC(buffer);
    EndPaint(frame, &paint_info);
}

static LRESULT CALLBACK frame_procedure(HWND frame, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_CREATE:
        command_bar = CommandBar_Create(instance, frame, IDC_COMMAND_BAR);
        CommandBar_AddAdornments(command_bar, 0, 0);
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT:
        paint_buffered(frame);
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
    create_fonts();
    window_class.lpfnWndProc = frame_procedure;
    window_class.hInstance = instance;
    window_class.lpszClassName = FRAME_CLASS;
    RegisterClassW(&window_class);
    HWND frame = CreateWindowExW(0, FRAME_CLASS, L"Drawing", WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, instance, NULL);
    if (!frame) {
        return 0;
    }
    ShowWindow(frame, show);
    UpdateWindow(frame);
    while (GetMessageW(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    destroy_fonts();
    return message.wParam;
}
