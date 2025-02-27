project(tactile-editor-app CXX)

add_executable(tactile)

target_sources(tactile
               PRIVATE
               "src/tactile/editor/main.cpp"
               )

tactile_prepare_target(tactile)

target_link_libraries(tactile
                      PRIVATE
                      tactile::editor
                      )
