project(tactile_editor_project CXX)

add_library(tactile_editor STATIC)

target_sources(tactile_editor
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "app/app.cppm"
               "cli/cli.cppm"
               "command/command.cppm"
               "command/command-interfaces.cppm"
               "command/command-stack.cppm"
               "tactile_editor.cppm"

               PRIVATE
               "app/launch.cpp"
               "cli/cli.cpp"
               "command/command_stack.cpp"
               )

tactile_set_target_properties(tactile_editor)

target_link_libraries(tactile_editor
                      PUBLIC
                      tactile_interface_target
                      tactile_core

                      PRIVATE
                      argparse::argparse
                      )
