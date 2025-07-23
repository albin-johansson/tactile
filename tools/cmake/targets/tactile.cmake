# Copyright (C) 2025 Albin Johansson

add_executable(tactile)

tactile_set_target_properties(tactile)

target_sources(tactile
               PRIVATE
               "${TACTILE_SOURCE_DIR}/main/tactile.main.cpp"
               )

target_link_libraries(tactile
                      PRIVATE
                      tactile_interface_target
                      tactile_core
                      tactile_editor
                      )
