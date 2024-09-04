project(tactile-runtime-lib CXX)

add_library(tactile-runtime SHARED)
add_library(tactile::runtime ALIAS tactile-runtime)

target_sources(tactile-runtime
               PRIVATE
               "src/tactile/runtime/launcher.cpp"
               "src/tactile/runtime/logging.cpp"
               "src/tactile/runtime/plugin_instance.cpp"
               "src/tactile/runtime/protobuf_context.cpp"
               "src/tactile/runtime/runtime.cpp"
               "src/tactile/runtime/sdl_context.cpp"
               "src/tactile/runtime/window.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/runtime/api.hpp"
               "inc/tactile/runtime/launcher.hpp"
               "inc/tactile/runtime/logging.hpp"
               "inc/tactile/runtime/plugin.hpp"
               "inc/tactile/runtime/plugin_instance.hpp"
               "inc/tactile/runtime/protobuf_context.hpp"
               "inc/tactile/runtime/runtime.hpp"
               "inc/tactile/runtime/sdl_context.hpp"
               "inc/tactile/runtime/window.hpp"
               )

tactile_prepare_target(tactile-runtime)

target_compile_definitions(tactile-runtime
                           PRIVATE
                           "TACTILE_BUILDING_RUNTIME"
                           "$<$<BOOL:TACTILE_BUILD_ZLIB_COMPRESSION>:TACTILE_ENABLE_ZLIB_COMPRESSION>"
                           "$<$<BOOL:TACTILE_BUILD_ZSTD_COMPRESSION>:TACTILE_ENABLE_ZSTD_COMPRESSION>"
                           "$<$<BOOL:TACTILE_BUILD_YAML_FORMAT>:TACTILE_ENABLE_YAML_FORMAT>"
                           "$<$<BOOL:TACTILE_BUILD_TILED_TMJ_FORMAT>:TACTILE_ENABLE_TILED_TMJ_FORMAT>"
                           "$<$<BOOL:TACTILE_BUILD_TILED_TMX_FORMAT>:TACTILE_ENABLE_TILED_TMX_FORMAT>"
                           "$<$<BOOL:TACTILE_BUILD_GODOT_TSCN_FORMAT>:TACTILE_ENABLE_GODOT_TSCN_FORMAT>"
                           "$<$<BOOL:TACTILE_BUILD_OPENGL_RENDERER>:TACTILE_ENABLE_OPENGL_RENDERER>"
                           "$<$<BOOL:TACTILE_BUILD_VULKAN_RENDERER>:TACTILE_ENABLE_VULKAN_RENDERER>"
                           )

target_link_libraries(tactile-runtime
                      PUBLIC
                      tactile::base

                      PRIVATE
                      tactile::core
                      SDL2::SDL2
                      )
