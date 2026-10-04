#include <windows.h>

#include <velo/cxx/format.h>
#include <velo/cxx/gdi.h>
#include <velo/cxx/handle.h>
#include <velo/cxx/memory.h>
#include <velo/cxx/optional.h>
#include <velo/cxx/result.h>
#include <velo/cxx/utf8.h>
#include <velo/cxx/window.h>

static_assert(velo::wstring_view(L"window").starts_with(L"win"));
static_assert(velo::wstring_view(L"window").find(L"dow") == 3);
static_assert(velo::wstring<8>(L"overflowing").size() == 7);
static_assert(velo::to_utf8<16>(L"café").size() == 5);
static_assert(velo::to_wstring<16>("caf\xc3\xa9") == velo::wstring_view(L"café"));
static_assert(velo::to_wstring<16>("\xf0\x9f\x98\x80").size() == 2);
static_assert(velo::to_utf8<16>(velo::to_wstring<16>("\xf0\x9f\x98\x80")) == velo::string_view("\xf0\x9f\x98\x80"));
static_assert(velo::to_wstring<16>("a\xffz") == velo::wstring_view(L"a\xfffdz"));
static_assert(velo::to_wstring<16>("\xed\xa0\x80") == velo::wstring_view(L"\xfffd"));
static_assert(velo::to_wstring<3>("ab\xf0\x9f\x98\x80").size() == 2);
static_assert(velo::to_utf8<4>(L"a€").size() == 1);
static_assert(velo::error::from_hresult(E_FAIL).is_hresult());
static_assert(velo::error{ERROR_FILE_NOT_FOUND}.hresult() == static_cast<HRESULT>(0x80070002));
static_assert(!velo::check_hresult(E_FAIL));
static_assert(velo::optional<int>(3).value_or(4) == 3);
static_assert(velo::optional<int>().value_or(4) == 4);

enum class mode : unsigned char { first, second };

int format_everything(int count, const wchar_t *name, unsigned short code, mode current, const char *narrow) {
    velo::wstring<16> label = L"label";
    auto first = velo::format<64>(L"Taps: %d\nTap anywhere, Esc to quit", count);
    auto second = velo::format(L"%s has %u items, code %04X, %c, %d, %-8s|", name, 3u, code, L'x', current, label);
    auto third = velo::format<32>(L"%.10s %S %%", name, narrow);
    velo::wstring<24> fourth;
    bool fitted = velo::format_to(fourth, L"%#x %lu", 255, 7ul);
    return static_cast<int>(first.size() + second.size() + third.size() + fourth.size()) + fitted;
}

namespace {

class every_handler : public velo::window<every_handler> {
public:
    static constexpr const wchar_t *class_name = L"EveryHandler";
    static constexpr UINT class_style = CS_DBLCLKS;

    static HBRUSH background() {
        return static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
    }

    bool on_create() {
        return true;
    }

    void on_paint(velo::paint_dc &dc) {
        auto pen = velo::create_pen(PS_SOLID, 1, RGB(0, 0, 0));
        auto brush = velo::create_solid_brush(RGB(255, 255, 255));
        auto selected_pen = dc.select(pen);
        auto selected_brush = dc.select(brush);
        dc.rectangle(client_rect());
        POINT line[] = {{0, 0}, {10, 10}};
        dc.polyline(line);
        dc.text_out({2, 2}, L"text");
    }

    void on_tap(velo::point where) {
        velo::window_dc dc(handle());
        dc.ellipse({where.x - 2, where.y - 2, where.x + 2, where.y + 2});
    }

    bool on_key(UINT key) {
        return key == VK_ESCAPE;
    }

    void on_command(WORD id, WORD code, HWND control) {
        last_command = id + code + (control ? 1 : 0);
    }

    bool on_message(UINT message, WPARAM, LPARAM, LRESULT &result) {
        if (message == WM_ERASEBKGND) {
            result = 1;
            return true;
        }
        return false;
    }

    void on_destroy() {
        PostQuitMessage(0);
    }

private:
    int last_command = 0;
};

class no_handlers : public velo::window<no_handlers> {
public:
    static constexpr const wchar_t *class_name = L"NoHandlers";
};

}

int use_everything(HINSTANCE instance) {
    auto event = velo::create_event(false, false);
    if (!event) {
        return static_cast<int>(event.error().code);
    }
    velo::unique_handle moved = velo::move(*event);
    auto file = velo::create_file(L"\\missing.txt", GENERIC_READ, 0, OPEN_EXISTING);
    auto buffer = velo::local_alloc<WCHAR>(32);
    if (!buffer) {
        return -1;
    }
    buffer[0] = L'x';
    if (!every_handler::register_class(instance) || !no_handlers::register_class(instance)) {
        return -2;
    }
    every_handler first;
    no_handlers second;
    if (!first.create(instance, L"Every", WS_VISIBLE, velo::screen_rect()) || !second.create(instance, L"None", 0, RECT{0, 0, 10, 10})) {
        return -3;
    }
    return velo::run_message_loop() + (file ? 1 : 0) + (moved ? 1 : 0);
}
