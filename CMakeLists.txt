cmake_minimum_required(VERSION 3.20)
project(sdl_func_drawer C)

set(CMAKE_C_STANDARD 11)
find_package(SDL2 REQUIRED)
find_package(SDL_ttf REQUIRED)

add_executable(sdl_func_drawer
        src/main.c src/sdl_func_drawer.c src/sdl_func_drawer.h)
target_link_libraries(sdl_func_drawer
        SDL2
        SDL2_ttf
        m)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})