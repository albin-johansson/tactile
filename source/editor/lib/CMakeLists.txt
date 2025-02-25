project(tactile-editor-lib CXX)

add_library(tactile_editor STATIC)
add_library(tactile::editor ALIAS tactile_editor)

target_sources(tactile_editor
               PUBLIC FILE_SET HEADERS BASE_DIRS inc FILES
               "inc/tactile/editor/cmd/command.hpp"
               "inc/tactile/editor/cmd/command_stack.hpp"

               PRIVATE
               "src/tactile/editor/cmd/command_stack.cpp"
               )

tactile_prepare_target(tactile_editor)

target_link_libraries(tactile_editor
                      PRIVATE
                      tactile_basic_target
                      )
