project(tactile_editor_module_tests CXX)

find_package(GTest CONFIG REQUIRED)

add_executable(tactile_editor_tests)

target_sources(tactile_editor_tests
               PRIVATE
               "command/command_stack.test.cpp"
               "tactile_editor_tests.main.cpp"
               )

tactile_set_target_properties(tactile_editor_tests)

target_link_libraries(tactile_editor_tests
                      PRIVATE
                      tactile_interface_target
                      tactile_editor
                      GTest::gtest
                      )
