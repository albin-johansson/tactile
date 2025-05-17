project(tactile_app_project CXX)

add_executable(tactile_app)

target_sources(tactile_app
               PRIVATE
               "tactile.main.cpp"
               )

tactile_set_target_properties(tactile_app)

set_target_properties(tactile_app
                      PROPERTIES
                      OUTPUT_NAME "tactile"
                      )

target_link_libraries(tactile_app
                      PRIVATE
                      tactile_interface_target
                      tactile_core
                      tactile_editor
                      )
