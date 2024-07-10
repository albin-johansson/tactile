project(tactile-runtime-lib CXX)

add_library(tactile-runtime SHARED)
add_library(tactile::runtime ALIAS tactile-runtime)

target_sources(tactile-runtime
               PRIVATE
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/api.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/launcher.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/launcher.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/plugin.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/plugin_instance.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/plugin_instance.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/protobuf_context.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/protobuf_context.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/runtime.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/runtime.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/sdl_context.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/sdl_context.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/tactile_app.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/tactile_app.cpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/window.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/window.cpp"
               )

tactile_prepare_target(tactile-runtime)

target_include_directories(tactile-runtime
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"

                           PRIVATE
                           "${PROJECT_SOURCE_DIR}/src"
                           )

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
                      imgui::imgui
                      )
