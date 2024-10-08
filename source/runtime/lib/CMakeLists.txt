project(tactile-runtime-lib CXX)

add_library(tactile-runtime SHARED)
add_library(tactile::runtime ALIAS tactile-runtime)

target_sources(tactile-runtime
               PRIVATE
               "src/tactile/runtime/command_line_options.cpp"
               "src/tactile/runtime/launcher.cpp"
               "src/tactile/runtime/logging.cpp"
               "src/tactile/runtime/plugin_instance.cpp"
               "src/tactile/runtime/protobuf_context.cpp"
               "src/tactile/runtime/runtime.cpp"
               "src/tactile/runtime/sdl_context.cpp"
               "src/tactile/runtime/window.cpp"
               "src/tactile/document_factory.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/runtime/api.hpp"
               "inc/tactile/runtime/command_line_options.hpp"
               "inc/tactile/runtime/document_factory.hpp"
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
                           )

target_link_libraries(tactile-runtime
                      PUBLIC
                      tactile::base

                      PRIVATE
                      tactile::core
                      SDL2::SDL2
                      argparse::argparse
                      )
