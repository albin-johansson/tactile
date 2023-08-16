cmake_minimum_required(VERSION 3.16)

project(tactile-editor CXX)

file(GLOB_RECURSE TACTILE_EDITOR_SOURCE_FILES
     CONFIGURE_DEPENDS
     ${TACTILE_INCLUDE_DIR}/tactile/editor/*.hpp
     ${TACTILE_INCLUDE_DIR}/tactile/editor/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.hpp
     ${PROJECT_SOURCE_DIR}/**/*.cpp
     )

add_library(${TACTILE_EDITOR} SHARED ${TACTILE_EDITOR_SOURCE_FILES})

tactile_set_properties(${TACTILE_EDITOR})
tactile_set_compiler_warnings(${TACTILE_EDITOR})

target_include_directories(${TACTILE_EDITOR} PUBLIC ${TACTILE_INCLUDE_DIR})

target_compile_definitions(${TACTILE_EDITOR}
                           PRIVATE
                           TACTILE_BUILDING_EDITOR
                           )

target_precompile_headers(${TACTILE_EDITOR} PRIVATE ${TACTILE_STANDARD_HEADERS})

target_link_libraries(${TACTILE_EDITOR}
                      PRIVATE
                      ${TACTILE_CORE}
                      SDL2::SDL2
                      imgui::imgui
                      )