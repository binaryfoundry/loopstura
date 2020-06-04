#pragma once

#if defined (IS_PLATFORM_DARWIN)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_image.h>
#endif
