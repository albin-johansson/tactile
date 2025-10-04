# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

add_library(tactile_editor STATIC)

tactile_init_target(tactile_editor)

target_link_libraries(tactile_editor
                      PUBLIC
                      dear_imgui
                      SDL3::SDL3
                      )

target_sources(tactile_editor
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "${PROJECT_SOURCE_DIR}/include" FILES
               "${PROJECT_SOURCE_DIR}/include/tactile/editor/command/command.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/editor/command/command_stack.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/editor/event/event_id.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/editor/event/event_queue.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/editor/event/events.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/editor/app.hpp"

               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/tactile/editor/command/command_stack.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/editor/event/event_queue.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/editor/app.cpp"
               )
