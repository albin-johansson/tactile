project(tactile-opengl-renderer-lib CXX)

add_library(tactile-opengl-renderer SHARED EXCLUDE_FROM_ALL)
add_library(tactile::opengl_renderer ALIAS tactile-opengl-renderer)

target_sources(tactile-opengl-renderer
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl_renderer/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl_renderer/opengl_error.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl_renderer/opengl_error.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl_renderer/opengl_imgui.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl_renderer/opengl_imgui.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl_renderer/opengl_renderer.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl_renderer/opengl_renderer.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl_renderer/opengl_renderer_plugin.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl_renderer/opengl_renderer_plugin.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/opengl_renderer/opengl_texture.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/opengl_renderer/opengl_texture.cpp"
               )

tactile_prepare_target(tactile-opengl-renderer)

target_compile_definitions(tactile-opengl-renderer
                           PRIVATE
                           "TACTILE_BUILDING_OPENGL_RENDERER"
                           )

target_include_directories(tactile-opengl-renderer
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"

                           SYSTEM PRIVATE
                           "${Stb_INCLUDE_DIR}"
                           )

target_link_libraries(tactile-opengl-renderer
                      PUBLIC
                      tactile::base
                      tactile::runtime

                      PRIVATE
                      OpenGL::GL
                      glad::glad
                      SDL2::SDL2
                      imgui::imgui
                      )
