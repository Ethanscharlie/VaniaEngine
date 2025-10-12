cmake_minimum_required(VERSION 3.20)                                                                 
project(index)       

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

add_executable(index
  src/main.cpp
  src/App.cpp
  src/EntityPanel.cpp
  src/WorldPanel.cpp
  src/EntityDefEditor.cpp
)

target_include_directories(index PUBLIC
  src
  external/sol
)

add_subdirectory(external/SDL)
# find_package(SDL3 REQUIRED)
target_link_libraries(index PUBLIC SDL3::SDL3)

add_subdirectory(external/SDL_image)
# find_package(SDL3_image REQUIRED)
target_link_libraries(index PUBLIC SDL3_image::SDL3_image)

add_subdirectory(external/SDL_mixer)
# find_package(SDL3_mixer REQUIRED)
target_link_libraries(index PUBLIC SDL3_mixer::SDL3_mixer)

add_subdirectory(external/SDL_ttf)
# find_package(SDL3_ttf REQUIRED)
target_link_libraries(index PUBLIC SDL3_ttf::SDL3_ttf)

add_subdirectory(external/imgui)
target_link_libraries(index PUBLIC imgui imgui_impl_sdl3 imgui_impl_sdlrenderer3)

find_package(Lua REQUIRED)

if(Lua_FOUND AND NOT TARGET Lua::Lua)
  add_library(Lua::Lua INTERFACE IMPORTED)
  set_target_properties(
    Lua::Lua
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
      INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
  )
endif()

target_link_libraries(index PRIVATE Lua::Lua)
