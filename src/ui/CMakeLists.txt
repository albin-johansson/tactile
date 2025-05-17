project(tactile_ui_project CXX)

add_library(tactile_ui STATIC)

target_sources(tactile_ui
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "ui.cppm"
               "ui-imgui.cppm"
               )

tactile_set_target_properties(tactile_ui)

target_link_libraries(tactile_ui
                      PUBLIC
                      tactile_interface_target
                      tactile_core
                      imgui::imgui
                      )

if (TACTILE_ENABLE_OPENGL)
  target_compile_definitions(tactile_ui PRIVATE "TACTILE_ENABLE_OPENGL")
endif ()
