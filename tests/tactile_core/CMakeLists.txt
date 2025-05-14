project(tactile_core_tests_project CXX)

find_package(GTest CONFIG REQUIRED)

add_executable(tactile_core_tests)

target_sources(tactile_core_tests
               PRIVATE
               "common/error.test.cpp"
               "common/strings.test.cpp"
               "io/base64.test.cpp"
               "layer/annotation_layer.test.cpp"
               "layer/group_layer.test.cpp"
               "layer/layer.test.cpp"
               "layer/layer_info.test.cpp"
               "layer/tile_layer.test.cpp"
               "meta/attr.test.cpp"
               "numeric/casts.test.cpp"
               "numeric/checked.test.cpp"
               "numeric/vec.test.cpp"
               "util/defer.test.cpp"
               "util/validation.test.cpp"
               "tactile_tests.main.cpp"
               )

tactile_set_target_properties(tactile_core_tests)

target_link_libraries(tactile_core_tests
                      PRIVATE
                      tactile_interface_target
                      tactile_core
                      GTest::gtest
                      )
