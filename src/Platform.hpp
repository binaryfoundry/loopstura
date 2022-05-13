#pragma once

#if defined (IS_PLATFORM_IOS)
#include <CoreFoundation/CoreFoundation.h>
#elif defined (IS_PLATFORM_OSX)
#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#elif defined (IS_PLATFORM_WIN)
#elif defined (IS_PLATFORM_EMSCRIPTEN)
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

static const char* IS_APPLICATION_NAME = "MISTURA";
