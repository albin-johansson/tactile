project(tactile-opengl-renderer-test CXX)

add_executable(tactile-opengl-renderer-test)

target_sources(tactile-opengl-renderer-test
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/main.cpp"
               "${PROJECT_SOURCE_DIR}/src/opengl_error_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/opengl_renderer_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/opengl_texture_test.cpp"
               )

tactile_prepare_target(tactile-opengl-renderer-test)

target_link_libraries(tactile-opengl-renderer-test
                      PUBLIC
                      tactile::opengl_renderer
                      GTest::gtest

                      PRIVATE
                      glad::glad
                      SDL2::SDL2
                      imgui::imgui
                      )
