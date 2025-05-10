project(tactile_editor_module CXX)

find_package(argparse CONFIG REQUIRED)

add_library(tactile_editor STATIC)

target_sources(tactile_editor
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "app/app.cppm"
               "cli/cli.cppm"
               "command/command.cppm"
               "command/command-interfaces.cppm"
               "tactile_editor.cppm"

               PRIVATE
               "app/launch.cpp"
               "cli/cli.cpp"
               )

tactile_prepare_target(tactile_editor)

target_link_libraries(tactile_editor
                      PUBLIC
                      tactile

                      PRIVATE
                      argparse::argparse
                      )
