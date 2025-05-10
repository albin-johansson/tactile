project(tactile_app CXX)

add_executable(tactile_app)

target_sources(tactile_app
               PRIVATE
               "tactile.main.cpp"
               )

tactile_prepare_target(tactile_app)

set_target_properties(tactile_app
                      PROPERTIES
                      OUTPUT_NAME "tactile"
                      )

target_link_libraries(tactile_app
                      PRIVATE
                      tactile
                      tactile_editor
                      )
