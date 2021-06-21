cmake_minimum_required(VERSION 3.12)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    application.cpp
    application.hpp
    imgui_wrapper.cpp
    imgui_wrapper.hpp
    init_open_gl_attributes.cpp
    init_open_gl_attributes.hpp
    main.cpp

    gui/show_grid.cpp
    gui/show_grid.hpp
    gui/show_gui.cpp
    gui/show_gui.hpp
    gui/show_map_viewport.cpp
    gui/show_map_viewport.hpp
    gui/show_properties.cpp
    gui/show_properties.hpp
    gui/show_tilesets.cpp
    gui/show_tilesets.hpp
    )

list(APPEND TACTILE_SOURCE_FILES ${IMGUI_SOURCES})

add_library(libTactile ${TACTILE_SOURCE_FILES})

add_definitions(-DCENTURION_NO_SDL_TTF)
add_definitions(-DCENTURION_NO_SDL_MIXER)
add_definitions(-DCENTURION_NO_SDL_IMAGE)

target_include_directories(libTactile
    PUBLIC ${TACTILE_ROOT_DIR}/src

    SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS}
    SYSTEM PUBLIC ${OPENGL_INCLUDE_DIRS}
    SYSTEM PUBLIC ${SDL2_INCLUDE_DIR}
    SYSTEM PUBLIC ${LIBRARY_DIR}/imgui)

target_link_libraries(libTactile
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${SDL2_LIBRARY}
    PUBLIC libCenturion
    PUBLIC libNenya)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libTactile PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wdouble-promotion
      -Wswitch-default
      -Wswitch-enum
      -Wunused
      -Wuninitialized
      -Wsuggest-final-types
      -Wsuggest-final-methods
      -Wsuggest-override
      -Wduplicated-cond
      -Wconversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(libTactile PRIVATE
      /EHsc
      /MP
      /W3)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(libTactile PRIVATE
      -Xclang
      -Wall
      -Wextra
      -Wpedantic
      -Wdouble-promotion
      -Wswitch-default
      -Wswitch-enum
      -Wunused
      -Wuninitialized
      -Wconversion
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-unused-template
      -Wno-unused-macros
      -Wno-newline-eof
      -Wno-documentation-unknown-command)
endif ()

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(Tactile WIN32
      ${TACTILE_SOURCE_FILES}
      main.cpp)
else ()
  add_executable(Tactile
      ${TACTILE_SOURCE_FILES}
      main.cpp)
endif ()

add_dependencies(Tactile libTactile)

target_link_libraries(Tactile PUBLIC libTactile)

copy_directory_post_build(Tactile
    ${TACTILE_ROOT_DIR}/bin
    ${CMAKE_CURRENT_BINARY_DIR})