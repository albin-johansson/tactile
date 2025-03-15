project(tactile-null-renderer-plugin-lib CXX)

add_library(tactile_null_renderer SHARED)
add_library(tactile::null_renderer ALIAS tactile_null_renderer)

tactile_prepare_target(tactile_null_renderer)

target_sources(tactile_null_renderer
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/null_renderer/api.hpp"
               "inc/tactile/null_renderer/null_renderer.hpp"
               "inc/tactile/null_renderer/null_renderer_plugin.hpp"

               PRIVATE
               "src/tactile/null_renderer/null_renderer.cpp"
               "src/tactile/null_renderer/null_renderer_plugin.cpp"
               )

target_link_libraries(tactile_null_renderer
                      PUBLIC
                      tactile::core

                      PRIVATE
                      tactile_basic_target
                      )

target_compile_definitions(tactile_null_renderer
                           PRIVATE
                           TACTILE_BUILDING_NULL_RENDERER
                           )
