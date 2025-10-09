cmake_minimum_required(VERSION 3.20)
project(imgui)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

# Find SDL3 package (make sure it's installed)
find_package(SDL3 REQUIRED)

# Add ImGui source files
add_library(imgui STATIC
    imgui.cpp
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
)

# Include ImGui headers
target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

set(INSTALL_TARGETS imgui)
set(INSTALL_HEADERS imgui.h imconfig.h)

foreach(BACKEND sdl3 sdlrenderer3)
	set(NAME imgui_impl_${BACKEND})
	set(HEADER ${CMAKE_CURRENT_SOURCE_DIR}/backends/${NAME}.h)
	add_library(${NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/backends/${NAME}.cpp ${HEADER})
	target_link_libraries(${NAME} PUBLIC imgui)
	target_include_directories(${NAME} PUBLIC
	    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends>
	    $<INSTALL_INTERFACE:include>)
	LIST(APPEND INSTALL_TARGETS ${NAME})
	LIST(APPEND INSTALL_HEADERS ${HEADER})
endforeach()

target_link_libraries(imgui_impl_sdl3 PRIVATE SDL3::SDL3)
target_link_libraries(imgui_impl_sdlrenderer3 PRIVATE SDL3::SDL3)
# Only use SDL3 backend
# set(BACKEND sdl3)
# set(NAME imgui_impl_${BACKEND})
# set(HEADER ${CMAKE_CURRENT_SOURCE_DIR}/backends/${NAME}.h)

# add_library(${NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/backends/${NAME}.cpp ${HEADER})
# target_link_libraries(${NAME} PUBLIC imgui)

# Fixing the issue by providing absolute path to the backends folder
# target_include_directories(${NAME} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends>
#     $<INSTALL_INTERFACE:include>)

# LIST(APPEND INSTALL_TARGETS ${NAME})
# LIST(APPEND INSTALL_HEADERS ${HEADER})

# Link SDL3 for the SDL3 backend implementation
# target_link_libraries(imgui_impl_sdl3 PRIVATE SDL3::SDL3)

