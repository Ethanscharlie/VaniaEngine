cmake_minimum_required(VERSION 3.20)                                                                 
project(index)       

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

add_executable(index
  src/main.cpp
  src/App.cpp
)

target_include_directories(index PUBLIC
  src
)

add_subdirectory(external/SDL)
target_link_libraries(index PUBLIC SDL3::SDL3)

add_subdirectory(external/SDL_image)
target_link_libraries(index PUBLIC SDL3_image::SDL3_image)

add_subdirectory(external/SDL_mixer)
target_link_libraries(index PUBLIC SDL3_mixer::SDL3_mixer)

add_subdirectory(external/SDL_ttf)
target_link_libraries(index PUBLIC SDL3_ttf::SDL3_ttf)

add_subdirectory(external/imgui)
target_link_libraries(index PUBLIC imgui imgui_impl_sdl3 imgui_impl_sdlrenderer3)
