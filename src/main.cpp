#include <cstdio>
#include <cstdlib>
 
#define GLM_FORCE_RADIANS 1
#include <SDL.h>
#include <glad/glad.h>
 
#include "utils.h"
#include "graphics.h"

void sdl_error(const char* message)
{
    RUNTIME_ERROR("%s: %s\n", message, SDL_GetError());
}

void init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdl_error("Error initializing SDL");
    atexit(SDL_Quit);

    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

struct SDLWindow
{
    SDL_Window* window;
    SDL_GLContext context;
};

SDLWindow create_sdl_window(
    const char* title,
    int x, 
    int y,
    int w,
    int h,
    bool fullscreen
)
{
    SDLWindow new_window;
    Uint32 window_settings = SDL_WINDOW_OPENGL;

    if (fullscreen)
        window_settings |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    new_window.window = SDL_CreateWindow(
        title,
        x,
        y,
        w,
        h,
        window_settings
    );

    new_window.context = SDL_GL_CreateContext(new_window.window);
    if (new_window.context == NULL)
        sdl_error("GL context failed to create");

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    SDL_GL_SetSwapInterval(1);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

    return new_window;
}

int main(int argc, char* argv[])
{
    init_sdl();

    Graphics::Backend* backend = Graphics::init_backend(Graphics::OPENGL_4);

    SDLWindow window = create_sdl_window(
        "Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        false
    );

    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_GL_SwapWindow(window.window);
    }

    Graphics::deinit_backend(backend);

    return 0;
}