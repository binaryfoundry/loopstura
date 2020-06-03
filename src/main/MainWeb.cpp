#include "MainWeb.hpp"

#if defined(IS_PLATFORM_EMSCRIPTEN)

#include "FileSDL.hpp"

#include "../Platform.hpp"
#include "../Context.hpp"
#include "../main/SDL.hpp"
#include "../opengl/GL.hpp"
#include "../opengl/GLRenderer.hpp"

#include "../Client.hpp"

#include <iostream>
#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>

static SDL_Window* window = nullptr;

static uint32_t element_width;
static uint32_t element_height;
static bool is_full_screen = false;

static std::shared_ptr<Application::Client> client;
static std::shared_ptr<Application::OpenGL::GLRenderer> gl_renderer;

static int init_graphics();
static void run();

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    const char *error = SDL_GetError();
    if (*error != '\0')
    {
        std::cout << "SDL error: " << error;
        SDL_ClearError();
        return 1;
    }

    init_graphics();

    auto load_texture_2d = [](
        std::string id,
        uint8_t& p,
        uint32_t& w,
        uint32_t& h,
        std::shared_ptr<std::vector<uint8_t>> d)
    {
        FileLoadTexture2D(id, p, w, h, d);
    };

    Application::ContextPtr context = std::make_shared<Application::Context>(
        load_texture_2d);

    std::function<void()> swap = [&]() {};

    gl_renderer = std::make_shared<Application::OpenGL::GLRenderer>(
        context,
        swap,
        element_width,
        element_height);

    client = std::make_shared<Application::Client>(
        context,
        gl_renderer);

    run();

    return 0;
}

static int init_graphics()
{
    double cssW, cssH;
    emscripten_get_element_css_size(0, &cssW, &cssH);
    element_width = static_cast<uint32_t>(cssW);
    element_height = static_cast<uint32_t>(cssH);

    window = SDL_CreateWindow(
        IS_APPLICATION_NAME,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        element_width,
        element_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.alpha = 0;
    attr.depth = 1;
    attr.stencil = 0;
    attr.antialias = 1;
    attr.preserveDrawingBuffer = 0;
    attr.preferLowPowerToHighPerformance = 0;
    attr.failIfMajorPerformanceCaveat = 0;
    attr.enableExtensionsByDefault = 1;
    attr.premultipliedAlpha = 0;
    attr.explicitSwapControl = 0;
    attr.majorVersion = 3;
    attr.minorVersion = 0;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context(
        0, &attr);

    emscripten_webgl_make_context_current(
        ctx);

    return 0;
}

static void update()
{
    client->Update();
    client->Render();
}

EM_BOOL em_fullscreen_callback(
    int eventType,
    const EmscriptenFullscreenChangeEvent *fullscreenChangeEvent,
    void *userData)
{
    is_full_screen = fullscreenChangeEvent->isFullscreen;
    if (is_full_screen)
    {
        int fullscreenWidth = (int)(emscripten_get_device_pixel_ratio()
            * fullscreenChangeEvent->screenWidth + 0.5);
        int fullscreenHeight = (int)(emscripten_get_device_pixel_ratio()
            * fullscreenChangeEvent->screenHeight + 0.5);

        element_width = static_cast<uint32_t>(fullscreenWidth);
        element_height = static_cast<uint32_t>(fullscreenHeight);
        gl_renderer->Resize(
            element_width,
            element_height);
    }
    return false;
}

EM_BOOL em_pointerlock_callback(
    int eventType,
    const EmscriptenPointerlockChangeEvent *pointerEvent,
    void *userData)
{
    return false;
}

EM_BOOL em_mouse_click_callback(
    int eventType,
    const EmscriptenMouseEvent *mouseEvent,
    void *userData)
{
    return false;
}

EM_BOOL em_mouse_move_callback(
    int eventType,
    const EmscriptenMouseEvent *mouseEvent,
    void *userData)
{
    return false;
}

EM_BOOL on_canvassize_changed(int eventType, const void *reserved, void *userData)
{
    int w, h, fs;
    double cssW, cssH;
    emscripten_get_element_css_size(0, &cssW, &cssH);
    element_width = static_cast<uint32_t>(cssW);
    element_height = static_cast<uint32_t>(cssH);

    gl_renderer->Resize(
        element_width,
        element_height);

    printf("Canvas resized: WebGL RTT size: %dx%d, canvas CSS size: %02gx%02g\n", w, h, cssW, cssH);
    return 0;
}

void enter_full_screen()
{
    if (!is_full_screen)
    {
        EM_ASM(JSEvents.inEventHandler = true);
        EM_ASM(JSEvents.currentEventHandler = { allowsDeferredCalls:true });
        EmscriptenFullscreenStrategy s;
        memset(&s, 0, sizeof(s));
        s.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_HIDEF;
        s.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_HIDEF;
        s.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
        s.canvasResizedCallback = on_canvassize_changed;
        EMSCRIPTEN_RESULT ret = emscripten_request_fullscreen_strategy(0, 1, &s);
        is_full_screen = true;
    }
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
        void external_enter_full_screen()
    {
        enter_full_screen();
    }
}

EM_BOOL em_key_down_callback(
    int eventType,
    const EmscriptenKeyboardEvent *keyEvent,
    void *userData)
{
    return false;
}

EM_BOOL em_key_up_callback(
    int eventType,
    const EmscriptenKeyboardEvent *keyEvent,
    void *userData)
{
    return false;
}

EM_BOOL em_resize_callback(
    int eventType,
    const EmscriptenUiEvent *e,
    void *userData)
{
    double cssW, cssH;
    emscripten_get_element_css_size(0, &cssW, &cssH);
    element_width = static_cast<uint32_t>(cssW);
    element_height = static_cast<uint32_t>(cssH);
    gl_renderer->Resize(
        element_width,
        element_height);
    return 0;
}

static void run()
{
    emscripten_set_pointerlockchange_callback(
        NULL, NULL, true, em_pointerlock_callback);
    emscripten_set_fullscreenchange_callback(
        NULL, NULL, true, em_fullscreen_callback);
    emscripten_set_click_callback(
        NULL, NULL, true, em_mouse_click_callback);
    emscripten_set_mousemove_callback(
        NULL, NULL, true, em_mouse_move_callback);
    emscripten_set_keydown_callback(
        NULL, NULL, true, em_key_down_callback);
    emscripten_set_keyup_callback(
        NULL, NULL, true, em_key_up_callback);
    emscripten_set_resize_callback(
        NULL, NULL, true, em_resize_callback);
    emscripten_set_element_css_size(
        NULL, element_width, element_height);

    emscripten_set_main_loop(
        update, 0, true);
}

#endif
