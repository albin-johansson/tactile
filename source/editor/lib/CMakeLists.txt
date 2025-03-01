project(tactile-editor-lib CXX)

find_package(SDL3 CONFIG REQUIRED)

add_library(tactile_editor STATIC)
add_library(tactile::editor ALIAS tactile_editor)

target_sources(tactile_editor
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/editor/cmd/command.hpp"
               "inc/tactile/editor/cmd/command_stack.hpp"
               "inc/tactile/editor/plugin/dynamic_library.hpp"
               "inc/tactile/editor/launcher.hpp"

               PRIVATE
               "src/tactile/editor/cmd/command_stack.cpp"
               "src/tactile/editor/plugin/dynamic_library.cpp"
               "src/tactile/editor/launcher.cpp"
               )

tactile_prepare_target(tactile_editor)

target_link_libraries(tactile_editor
                      PUBLIC
                      tactile::core

                      PRIVATE
                      tactile_basic_target
                      SDL3::SDL3
                      )
