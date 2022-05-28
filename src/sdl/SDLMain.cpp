#include "SDLMain.hpp"

#if defined(IS_PLATFORM_WIN) || defined(IS_PLATFORM_DARWIN)

#include "SDL.hpp"
#include "SDLFile.hpp"
#include "SDLImgui.hpp"

#include "../Platform.hpp"
#include "../Context.hpp"
#include "../opengl/GL.hpp"
#include "../opengl/GLRenderer.hpp"

#include "../Client.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>

#if defined (IS_PLATFORM_WIN)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

static SDL_GLContext gl;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Surface* screen = nullptr;

#if defined (IS_PLATFORM_WIN)
static EGLDisplay eglDisplay;
static EGLContext eglContext;
static EGLSurface eglSurface;
#endif

static int init_graphics();
static bool poll_events();
static void swap_buffers();

static int window_width = 1280;
static int window_height = 720;

static std::shared_ptr<Application::Client> client;
static std::shared_ptr<Application::Rendering::OpenGL::GLRenderer> gl_renderer;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Init(SDL_INIT_AUDIO);

    int flags = IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if ((initted&flags) != flags)
    {
        std::cout << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    sdl_imgui_initialise();

    int ret = init_graphics();

    if (ret != 0)
    {
        return ret;
    }

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

    std::function<void()> swap = [&]()
    {
        swap_buffers();
    };

    gl_renderer = std::make_shared<Application::Rendering::OpenGL::GLRenderer>(
        context,
        swap,
        window_width,
        window_height);

    client = std::make_shared<Application::Client>(
        context,
        gl_renderer);

    bool done = false;

    while (!done)
    {
        done = poll_events();

        // TODO proper game loop timing

        sdl_imgui_update_input(window);
        sdl_imgui_update_cursor();
        client->Update();
        client->Render();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    sdl_imgui_destroy();

    SDL_GL_DeleteContext(gl);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

static bool poll_events()
{
    SDL_Event event;

    SDL_PumpEvents();

    ImGuiIO& io = ImGui::GetIO();

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
            break;

        case SDL_APP_DIDENTERFOREGROUND:
            SDL_Log("SDL_APP_DIDENTERFOREGROUND");
            break;

        case SDL_APP_DIDENTERBACKGROUND:
            SDL_Log("SDL_APP_DIDENTERBACKGROUND");
            break;

        case SDL_APP_LOWMEMORY:
            SDL_Log("SDL_APP_LOWMEMORY");
            break;

        case SDL_APP_TERMINATING:
            SDL_Log("SDL_APP_TERMINATING");
            break;

        case SDL_APP_WILLENTERBACKGROUND:
            SDL_Log("SDL_APP_WILLENTERBACKGROUND");
            break;

        case SDL_APP_WILLENTERFOREGROUND:
            SDL_Log("SDL_APP_WILLENTERFOREGROUND");
            break;

        case SDL_MOUSEMOTION:
            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.x > 0) io.MouseWheelH += 1;
            if (event.wheel.x < 0) io.MouseWheelH -= 1;
            if (event.wheel.y > 0) io.MouseWheel += 1;
            if (event.wheel.y < 0) io.MouseWheel -= 1;
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    gl_renderer->Resize(
                        event.window.data1,
                        event.window.data2);
                    break;
                }
            }

        case SDL_MOUSEBUTTONUP:
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
            if (event.button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
            if (event.button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
            break;

        case SDL_TEXTINPUT:
            io.AddInputCharactersUTF8(event.text.text);
            break;

        case SDL_KEYDOWN:
            break;

        case SDL_KEYUP:
            int key = event.key.keysym.scancode;
            IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
            io.KeysDown[key] = (event.type == SDL_KEYDOWN);
            io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
            io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
            io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
#ifdef IS_PLATFORM_WIN
            io.KeySuper = false;
#else
            io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
#endif
            break;

        }
    }

    return false;
}

static int init_graphics()
{
#if defined (IS_PLATFORM_WIN)

    window = SDL_CreateWindow(
        IS_APPLICATION_NAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_SysWMinfo info;
    SDL_VERSION(
        &info.version);

    SDL_bool get_win_info = SDL_GetWindowWMInfo(
        window,
        &info);
    SDL_assert_release(
        get_win_info);

    EGLNativeWindowType hWnd = reinterpret_cast<EGLNativeWindowType>(
        info.info.win.window);

    EGLint err;
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;

    EGLint configAttribs[] =
    {
        EGL_RED_SIZE,       8,
        EGL_GREEN_SIZE,     8,
        EGL_BLUE_SIZE,      8,
        EGL_ALPHA_SIZE,     8,
        EGL_DEPTH_SIZE,     24,
        EGL_SAMPLE_BUFFERS, 1,
        EGL_SAMPLES,        2,
        EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,EGL_OPENGL_ES3_BIT_KHR,
        EGL_BUFFER_SIZE,    16,
        EGL_NONE
    };

    EGLint contextAttribs[] =
    {
        EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
        EGL_CONTEXT_MINOR_VERSION_KHR, 2,
        EGL_NONE
    };

    EGLint surfaceAttribs[] =
    {
        //EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
        //EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_SRGB_KHR,
        //EGL_COLORSPACE, EGL_COLORSPACE_sRGB,
        //EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_SRGB_KHR,
        EGL_NONE
    };

    display = eglGetDisplay(GetDC(hWnd));

    if (display == EGL_NO_DISPLAY)
        goto failed;
    if (!eglInitialize(
        display,
        &majorVersion,
        &minorVersion))
        goto failed;
    if (!eglBindAPI(EGL_OPENGL_ES_API))
        goto failed;
    if (!eglGetConfigs(
        display,
        NULL,
        0,
        &numConfigs))
        goto failed;
    if (!eglChooseConfig(
        display,
        configAttribs,
        &config,
        1,
        &numConfigs))
        goto failed;
    if (numConfigs != 1) {
        goto failed;
    }

    surface = eglCreateWindowSurface(
        display,
        config,
        hWnd,
        surfaceAttribs);
    if (surface == EGL_NO_SURFACE)
        goto failed;
    context = eglCreateContext(
        display,
        config,
        EGL_NO_CONTEXT,
        contextAttribs);
    if (context == EGL_NO_CONTEXT)
        goto failed;
    if (!eglMakeCurrent(display, surface, surface, context))
        goto failed;

failed:
    if ((err = eglGetError()) != EGL_SUCCESS)
    {
        std::cout << err << std::endl;
        return 1;
    }

    eglDisplay = display;
    eglSurface = surface;
    eglContext = context;

    auto r = eglSwapInterval(eglDisplay, 1);

    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

#else

    window_width = 800;
    window_height = 600;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
        IS_APPLICATION_NAME,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    gl = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl);

#endif

    const char *error = SDL_GetError();
    if (*error != '\0')
    {
        std::cout << error << std::endl;
        SDL_ClearError();
        return 1;
    }

    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;

    return 0;
}

static void swap_buffers()
{
#if defined(IS_PLATFORM_WIN)
    eglSwapBuffers(eglDisplay, eglSurface);
#else
    SDL_GL_SwapWindow(window);
#endif
}

#endif
