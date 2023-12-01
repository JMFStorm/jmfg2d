#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <Windows.h>
#include <glad/glad.h>

#include "globals.h"
#include "j_input.h"
#include "types.h"

typedef struct UserSettings {
    int window_width_px;
    int window_height_px;
} UserSettings;

UserSettings user_settings;

WNDCLASSEX window_class;
HWND window_handle;
HGLRC opengl_context;

GameInputs game_inputs_init()
{
    GameInputs inputs = {0};
    inputs.mouse1 = (ButtonState){.key=VK_LBUTTON,  .pressed=false, .is_down=false};
    inputs.mouse2 = (ButtonState){.key=VK_RBUTTON,  .pressed=false, .is_down=false};
    inputs.ctrl   = (ButtonState){.key=VK_CONTROL,  .pressed=false, .is_down=false};
    inputs.space  = (ButtonState){.key=VK_SPACE,    .pressed=false, .is_down=false};
    inputs.plus   = (ButtonState){.key=VK_ADD,      .pressed=false, .is_down=false};
    inputs.minus  = (ButtonState){.key=VK_SUBTRACT, .pressed=false, .is_down=false};
    return inputs;
}

const int BUTTON_INPUTS_COUNT = sizeof(GameInputs) / sizeof(ButtonState);

void get_button_state(ButtonState* button)
{
    bool is_pressed = GetAsyncKeyState(button->key) & 0x8000;
    button->pressed = !button->is_down && is_pressed;
    button->is_down = is_pressed;
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
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
        L"JMF Engine",          // lpWindowName: Pointer to a null-terminated string that specifies the window name.
        WS_OVERLAPPEDWINDOW,    // dwStyle: Window style.
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

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
    UserSettings user_settings = {0};
    user_settings.window_width_px = 1600;
    user_settings.window_height_px = 1200;

    window_class = register_and_create_window_class();
    window_handle = create_window(window_class, user_settings.window_width_px, user_settings.window_height_px);

    opengl_context = create_opengl_context();
    gladLoadGL();

    GameInputs g_inputs = game_inputs_init();

    init_shaders();

    ShowWindow(window_handle, cmd_show);
    UpdateWindow(window_handle);

    MSG msg;
    HDC device_context = GetDC(window_handle);

    float green = 0.0f;
    glViewport(0, 0, user_settings.window_width_px, user_settings.window_height_px);

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        ButtonState* buttons_ptr = (ButtonState*)&g_inputs;

        for (int i = 0; i < BUTTON_INPUTS_COUNT - 1; i++)
        {
            ButtonState* button_ptr = &buttons_ptr[i];
            get_button_state(button_ptr);
        }

        glClearColor(1.0f, green, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(simple_rect_shader.id);
        glBindVertexArray(simple_rect_shader.vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUseProgram(0);
        glBindVertexArray(0);

        SwapBuffers(device_context);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(opengl_context);

    ReleaseDC(window_handle, device_context);
    UnregisterClass(window_class.lpszClassName, window_class.hInstance);
    return 0;
}
