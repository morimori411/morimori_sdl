cmake_minimum_required(VERSION 3.14)
project(morimori_sdl CXX)
add_library(morimori_sdl
    src/Game.cpp
    src/Common/Common.cpp
    src/Common/Vector.cpp
    src/Collision/Collision.cpp
    src/Pictures/Fonts.cpp
    src/Pictures/Textures.cpp
    src/Pictures/Pictures.cpp
    src/Pictures/Camera.cpp
    src/Inputs/Keyboard.cpp
    src/Inputs/Mouse.cpp
)
target_compile_options(
    morimori_sdl PRIVATE
    $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>: /W4>
)
target_include_directories(morimori_sdl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)
target_link_libraries(morimori_sdl SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
