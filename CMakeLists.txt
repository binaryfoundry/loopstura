cmake_minimum_required(VERSION 3.8)

set(PROJECT imgui-shell)

project(${PROJECT})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(EMSCRIPTEN "Web Compilation" OFF)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SOURCES
    src/Platform.cpp
    src/Math.cpp
    src/Client.cpp
    src/Renderer.cpp)

set(HEADERS
    src/Platform.hpp
    src/Math.hpp
    src/Client.hpp
    src/Renderer.hpp)

set(SOURCES_MAIN
    src/main/SDL.cpp
    src/main/MainSDL.cpp
    src/main/MainWeb.cpp)

set(HEADERS_MAIN
    src/main/SDL.hpp
    src/main/MainSDL.hpp
    src/main/MainWeb.hpp)

set(SOURCES_OPENGL
    src/opengl/GL.cpp
    src/opengl/GLRenderer.cpp)

set(HEADERS_OPENGL
    src/opengl/GL.hpp
    src/opengl/GLRenderer.hpp)

SOURCE_GROUP("src" FILES ${SOURCES})
SOURCE_GROUP("src" FILES ${HEADERS})

SOURCE_GROUP("src\\main" FILES ${SOURCES_MAIN})
SOURCE_GROUP("src\\main" FILES ${HEADERS_MAIN})

SOURCE_GROUP("src\\opengl" FILES ${SOURCES_OPENGL})
SOURCE_GROUP("src\\opengl" FILES ${HEADERS_OPENGL})

if (MSVC)
    message(STATUS "Platform: MSVC")
    add_definitions(-DIS_PLATFORM_WIN)

    set(FFMPEG_PATH ${PROJECT_SOURCE_DIR}/lib/ffmpeg/windows)
    set(FFMPEG_LIB_PATH ${FFMPEG_PATH}/lib/x64)
    set(FFMPEG_BIN_PATH ${FFMPEG_PATH}/bin/x64)

    set(SDL2_PATH ${PROJECT_SOURCE_DIR}/lib/sdl/win/sdl2)
    set(SDL2_LIB_PATH ${SDL2_PATH}/lib/x64)

    set(SDL2_IMAGE_PATH ${PROJECT_SOURCE_DIR}/lib/sdl/win/sdl2-image)
    set(SDL2_IMAGE_LIB_PATH ${SDL2_IMAGE_PATH}/lib/x64)

    set(PVR_PATH ${PROJECT_SOURCE_DIR}/lib/pvr)
    set(PVR_LIB_PATH ${PVR_PATH}/Windows/x86_64)

    include_directories(${SDL2_PATH}/include)
    link_directories(${SDL2_LIB_PATH})

    include_directories(${SDL2_IMAGE_PATH}/include)
    link_directories(${SDL2_IMAGE_LIB_PATH})

    include_directories(${PVR_PATH}/Include)
    link_directories(${PVR_LIB_PATH})

elseif (EMSCRIPTEN)
    message(STATUS "Platform: EMSCRIPTEN")
    add_definitions(-DIS_PLATFORM_EMSCRIPTEN)

endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/lib/glm)

link_directories(
    ${FFMPEG_LIB_PATH})

add_executable(
    ${PROJECT}
    ${SOURCES}
    ${HEADERS}
    ${SOURCES_MAIN}
    ${HEADERS_MAIN}
    ${SOURCES_OPENGL}
    ${HEADERS_OPENGL})

target_include_directories(
    ${PROJECT}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include)

if (MSVC)
    target_include_directories(
        ${PROJECT}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/
        ${FFMPEG_PATH}/include
        ${SDL2_PATH}/include
        ${SDL2_IMAGE_PATH}/include
        ${PVR_PATH}/include)

    target_link_libraries(
        ${PROJECT}
        PRIVATE
        avcodec
        avformat
        avutil
        avdevice
        SDL2
        SDL2main
        SDL2_image
        libEGL
        libGLESv2
        libGLES_CM)

    add_custom_command(
        TARGET ${PROJECT}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${FFMPEG_BIN_PATH}"
        $<TARGET_FILE_DIR:${PROJECT}>)

    add_custom_command(
        TARGET ${PROJECT}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SDL2_LIB_PATH}"
        $<TARGET_FILE_DIR:${PROJECT}>)

    add_custom_command(
        TARGET ${PROJECT}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SDL2_IMAGE_LIB_PATH}"
        $<TARGET_FILE_DIR:${PROJECT}>)

    add_custom_command(
        TARGET ${PROJECT}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PVR_LIB_PATH}"
        $<TARGET_FILE_DIR:${PROJECT}>)

elseif (EMSCRIPTEN)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES SUFFIX ".html"
        LINK_FLAGS "--bind -s USE_SDL_IMAGE=2 -s USE_WEBGL2=1 -s USE_SDL_TTF=2 -s DEMANGLE_SUPPORT=1 -s USE_SDL=2 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=2 -s --emrun -std=c++11 -O3 --shell-file \"${PROJECT_SOURCE_DIR}/shell.html\" --use-preload-plugins")
    #--preload-file resources | -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=4 | -s \"BINARYEN_METHOD='native-wasm'\"

endif ()
