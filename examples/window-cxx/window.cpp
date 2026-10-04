#include <velo/cxx/format.h>
#include <velo/cxx/optional.h>
#include <velo/cxx/window.h>

namespace {

class tap_window : public velo::window<tap_window> {
public:
    static constexpr const wchar_t *class_name = L"VeloExampleWindowCxx";

    void on_paint(velo::paint_dc &dc) {
        auto text = velo::format<64>(L"Taps: %d\nTap anywhere, Esc to quit", tap_count);
        dc.draw_text(text, client_rect(), DT_CENTER | DT_WORDBREAK);
        if (last_tap) {
            dc.ellipse({last_tap->x - mark_size, last_tap->y - mark_size, last_tap->x + mark_size, last_tap->y + mark_size});
        }
    }

    void on_tap(velo::point where) {
        tap_count++;
        last_tap = where;
        invalidate();
    }

    void on_key(UINT key) {
        if (key == VK_ESCAPE) {
            destroy();
        }
    }

    void on_destroy() {
        PostQuitMessage(0);
    }

private:
    static constexpr int mark_size = 6;

    int tap_count = 0;
    velo::optional<velo::point> last_tap;
};

}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPWSTR command_line, int show) {
    tap_window window;
    if (!tap_window::register_class(instance) || !window.create(instance, L"Window", WS_VISIBLE | WS_CAPTION | WS_SYSMENU, velo::screen_rect())) {
        return 1;
    }
    window.focus();
    return velo::run_message_loop();
}
