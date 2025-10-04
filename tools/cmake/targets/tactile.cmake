# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

add_executable(tactile)

tactile_init_target(tactile)

target_link_libraries(tactile
                      PRIVATE
                      tactile_core
                      tactile_editor
                      )

target_sources(tactile
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/tactile/main/tactile.main.cpp"
               )
