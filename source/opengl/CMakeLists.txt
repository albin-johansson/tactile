project(tactile-opengl CXX)

add_library(tactile-opengl SHARED EXCLUDE_FROM_ALL)
add_library(tactile::opengl ALIAS tactile-opengl)

target_sources(tactile-opengl
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl/opengl_error.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl/opengl_error.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl/opengl_imgui.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl/opengl_imgui.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl/opengl_renderer.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl/opengl_renderer.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl/opengl_texture.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl/opengl_texture.cpp"
               )

tactile_prepare_target(tactile-opengl)

target_compile_definitions(tactile-opengl PRIVATE "TACTILE_BUILDING_OPENGL_RENDERER")

target_include_directories(tactile-opengl
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"

                           SYSTEM PRIVATE
                           "${Stb_INCLUDE_DIR}"
                           )

target_link_libraries(tactile-opengl
                      PUBLIC
                      tactile::base
                      tactile::render

                      PRIVATE
                      OpenGL::GL
                      glad::glad
                      SDL2::SDL2
                      imgui::imgui
                      )
