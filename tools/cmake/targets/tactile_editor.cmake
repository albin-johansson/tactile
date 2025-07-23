# Copyright (C) 2025 Albin Johansson

find_package(argparse CONFIG REQUIRED)

add_library(tactile_editor STATIC)

tactile_set_target_properties(tactile_editor)

target_sources(tactile_editor
               PUBLIC FILE_SET CXX_MODULES FILES
               "${TACTILE_MODULE_DIR}/tactile/editor/app/app.cppm"
               "${TACTILE_MODULE_DIR}/tactile/editor/cli/cli.cppm"
               "${TACTILE_MODULE_DIR}/tactile/editor/command/command.cppm"
               "${TACTILE_MODULE_DIR}/tactile/editor/command/command-interfaces.cppm"
               "${TACTILE_MODULE_DIR}/tactile/editor/command/command-stack.cppm"
               "${TACTILE_MODULE_DIR}/tactile/editor/editor.cppm"

               PRIVATE
               "${TACTILE_SOURCE_DIR}/tactile/editor/app/launch.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/editor/cli/cli.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/editor/command/command_stack.cpp"
               )

target_link_libraries(tactile_editor
                      PUBLIC
                      tactile_interface_target
                      tactile_core

                      PRIVATE
                      argparse::argparse
                      )
