project(tactile-render CXX)

add_library(tactile-render INTERFACE)
add_library(tactile::render ALIAS tactile-render)

target_sources(tactile-render
               INTERFACE
               "${PROJECT_SOURCE_DIR}/inc/tactile/render/renderer.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/render/texture.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/render/window.hpp"
               )

target_include_directories(tactile-render INTERFACE "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(tactile-render INTERFACE tactile::base)
