#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdarg.h>

#include "constants.h"
#include "globals.h"
#include "jimage.h"
#include "jgame.h"
#include "jinput.h"
#include "jmath.h"
#include "types.h"

WNDCLASSEX window_class;
HWND window_handle;
HGLRC opengl_context;
GameInputs inputs;

void debug_log(const char* str_format, ...)
{
#ifdef DEBUG_BUILD
    char d_msg[256];
    va_list args;
    va_start(args, str_format);
    vsnprintf(d_msg, sizeof(d_msg), str_format, args);
    OutputDebugStringA(d_msg);
    va_end(args);
#endif
}

Point get_window_size()
{
    RECT rect;
    Point size = {0};
    GetWindowRect(window_handle, &rect);
    size.x = rect.right - rect.left;
    size.y = rect.bottom - rect.top;
    return size;
}

void set_window_size(s32 width_px, s32 height_px)
{
    user_settings.window_width_px = width_px;
    user_settings.window_height_px = height_px;
    load_font(debug_font_ptr, get_debug_font_size(), "G:\\projects\\game\\jmfg2d\\resources\\fonts\\Inter-Regular.ttf");
    SetWindowPos(window_handle, NULL, 0, 0, width_px, height_px, SWP_NOMOVE | SWP_NOZORDER);
}

GameInputs game_inputs_init()
{
    GameInputs gi = {0};
    gi.mouse1 = (ButtonState){.key=VK_LBUTTON, .pressed=false, .is_down=false};
    gi.mouse2 = (ButtonState){.key=VK_RBUTTON, .pressed=false, .is_down=false};
    gi.ctrl   = (ButtonState){.key=VK_CONTROL, .pressed=false, .is_down=false};
    gi.space  = (ButtonState){.key=VK_SPACE,   .pressed=false, .is_down=false};
    gi.left   = (ButtonState){.key=VK_LEFT,    .pressed=false, .is_down=false};
    gi.right  = (ButtonState){.key=VK_RIGHT,   .pressed=false, .is_down=false};
    gi.up     = (ButtonState){.key=VK_UP,      .pressed=false, .is_down=false};
    gi.down   = (ButtonState){.key=VK_DOWN,    .pressed=false, .is_down=false};
    return gi;
}

const int BUTTON_INPUTS_COUNT = sizeof(GameInputs) / sizeof(ButtonState);

void get_button_state(ButtonState* button)
{
    bool is_pressed = GetAsyncKeyState(button->key) & 0x8000;
    button->pressed = !button->is_down && is_pressed;
    button->is_down = is_pressed;
}

void set_buttons_states()
{
    ButtonState* buttons_ptr = (ButtonState*)&inputs;

    for (int i = 0; i < BUTTON_INPUTS_COUNT; i++)
    {
        ButtonState* button_ptr = &buttons_ptr[i];
        get_button_state(button_ptr);
    }
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            s32 new_width = LOWORD(l_param);
            s32 new_height = HIWORD(l_param);
            set_draw_area(0, 0, new_width, new_height);
            return 0;

        case WM_PAINT:
            return 0;

        default:
            return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}

WNDCLASSEX register_and_create_window_class()
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = window_proc;
    wc.cbClsExtra = 0L;
    wc.cbWndExtra = 0L;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"MyWindowClass";
    wc.hIconSm = NULL;

    RegisterClassEx(&wc);
    return wc;
}

HWND create_window(WNDCLASSEX wc, int width, int height)
{
    HWND handle = CreateWindow(
        wc.lpszClassName,       // lpClassName: Pointer to a null-terminated string or a class atom.
        L"jmfg2d",          // lpWindowName: Pointer to a null-terminated string that specifies the window name.
        WS_OVERLAPPED 
        | WS_SYSMENU 
        | WS_CAPTION
        | WS_MINIMIZEBOX
        | WS_MAXIMIZEBOX,       // dwStyle: Window style.
        100, 100,               // x, y: Initial position of the window.
        width,                  // nWidth:
        height,                 // nHeight: Width and height of the window.
        NULL,                   // hWndParent: Handle to the parent or owner window.
        NULL,                   // hMenu: Handle to the window menu, or NULL if the class menu is to be used.
        wc.hInstance,           // hInstance: Handle to the instance of the module to be associated with the window.
        NULL                    // lpParam: Pointer to a value to be passed to the window through the CREATESTRUCT structure.
    );
    return handle;
}

HGLRC create_opengl_context()
{
    HDC dc = GetDC(window_handle);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
        1,                              // Version number
        PFD_DRAW_TO_WINDOW |            // Flags for pixel format
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,                  // Pixel type and color format
        32,                             // Preferred color depth (bits per pixel)
        0, 0, 0, 0, 0, 0,               // Color bits (ignored for RGBA)
        0,                              // No alpha buffer
        0,                              // Alpha bits (ignored for RGBA)
        0,                              // No accumulation buffer
        0, 0, 0, 0,                     // Accumulation bits (ignored)
        24,                             // Depth buffer (Z-buffer) size
        8,                              // Stencil buffer size
        0,                              // No auxiliary buffer
        PFD_MAIN_PLANE,                 // Main drawing layer
        0,                              // Reserved
        0, 0, 0                         // Layer masks (ignored)
    };

    int pixel_format = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pixel_format, &pfd);

    HGLRC context = wglCreateContext(dc);
    wglMakeCurrent(dc, context);
    ReleaseDC(window_handle, dc);
    return context;
}

u64 get_performance_frequency()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    u64 result = (u64)frequency.QuadPart;
    return result;
}

void init_performance_counter()
{
    system_data.perf_counter_freq = get_performance_frequency();
    system_data.elapsed_time_ms = 0;

    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    system_data.prev_perf_counter = (u64)time.QuadPart;

    frame_data.deltatime_ms = 0.0f;
}

void update_performance_counter()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    system_data.perf_counter = (u64)time.QuadPart;

    f64 diff = (f64)(system_data.perf_counter - system_data.prev_perf_counter);
    frame_data.deltatime_ms = (diff * 1000.0f) / system_data.perf_counter_freq;

    system_data.elapsed_time_ms += frame_data.deltatime_ms;
    system_data.prev_perf_counter = system_data.perf_counter;
}

u32 test_texture_id;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
    user_settings.window_width_px = 1600;
    user_settings.window_height_px = 1200;

    init_performance_counter();

    window_class = register_and_create_window_class();
    window_handle = create_window(window_class, user_settings.window_width_px, user_settings.window_height_px);

    opengl_context = create_opengl_context();
    gladLoadGL();

    inputs = game_inputs_init();

    debug_font_ptr = malloc(sizeof(FontData));
    load_font(debug_font_ptr, get_debug_font_size(), "G:\\projects\\game\\jmfg2d\\resources\\fonts\\Inter-Regular.ttf");

    init_shaders();

    set_vertical_flip_image_load(true);
    test_texture_id = load_image_to_texture("G:\\projects\\game\\jmfg2d\\resources\\images\\bricks_reclaimed.png");

    ShowWindow(window_handle, cmd_show);
    UpdateWindow(window_handle);

    set_window_size(user_settings.window_width_px, user_settings.window_height_px);

    MSG msg;
    HDC device_context = GetDC(window_handle);

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        set_buttons_states();

        if (inputs.right.pressed)
        {
            Point window_size = get_window_size();
            window_size.x += 100;
            set_window_size(window_size.x, window_size.y);
        }
        else if (inputs.left.pressed)
        {
            Point window_size = get_window_size();
            window_size.x -= 100;
            set_window_size(window_size.x, window_size.y);
        }
        else if (inputs.down.pressed)
        {
            Point window_size = get_window_size();
            window_size.y += 100;
            set_window_size(window_size.x, window_size.y);
        }
        else if (inputs.up.pressed)
        {
            Point window_size = get_window_size();
            window_size.y -= 100;
            set_window_size(window_size.x, window_size.y);
        }

        update_performance_counter();

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        vec2 rect_offset2 = { 0.0f, -0.5f };
        append_rect(rect_offset2);
        draw_rects(test_texture_id);

        vec3 dot_color = { 1, 1, 1 };
        vec3 start = { 0, 0, 0 };

        for (int i = 0; i < 10; i++)
        {
            vec3 end = { -0.75f, 0, 0 };

            f32 val = 1.0f * i * 0.1f;
            f32 y = -1.0f + val;
            end.y = y;
            append_line(start, end, dot_color);
        }

        draw_lines(1.5f);

        print_debug_info();

        SwapBuffers(device_context);
        system_data.frames_drawn++;
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(opengl_context);

    ReleaseDC(window_handle, device_context);
    UnregisterClass(window_class.lpszClassName, window_class.hInstance);
    return 0;
}

