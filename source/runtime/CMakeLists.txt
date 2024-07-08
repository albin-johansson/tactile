project(tactile-runtime CXX)

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
               "${PROJECT_SOURCE_DIR}/inc/tactile/runtime/window.hpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/runtime/window.cpp"
               )

tactile_prepare_target(tactile-runtime)

target_include_directories(tactile-runtime
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

target_compile_definitions(tactile-runtime
                           PRIVATE
                           "TACTILE_BUILDING_RUNTIME"
                           "TACTILE_ENABLE_ZLIB_COMPRESSION"
                           "TACTILE_ENABLE_ZSTD_COMPRESSION"
                           "TACTILE_ENABLE_OPENGL_RENDERER"
                           )

target_link_libraries(tactile-runtime
                      PUBLIC
                      tactile::base

                      PRIVATE
                      tactile::core
                      protobuf::libprotobuf
                      SDL2::SDL2
                      imgui::imgui
                      )
