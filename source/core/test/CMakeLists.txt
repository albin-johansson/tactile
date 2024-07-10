project(tactile-core-test CXX)

add_executable(tactile-core-test)

target_sources(tactile-core-test
               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/cmd/meta/create_property_command_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/cmd/meta/remove_property_command_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/cmd/meta/rename_property_command_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/cmd/meta/update_property_command_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/cmd/command_stack_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/debug/validation_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/debug/validation_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/entity/registry_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/event/event_dispatcher_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/io/ini_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/layer/group_layer_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/layer/layer_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/layer/object_layer_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/layer/object_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/layer/tile_layer_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/layer/tile_matrix_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/map/map_spec_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/map/map_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/meta/attribute_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/meta/attribute_type_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/meta/color_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/model/settings_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/numeric/random_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/platform/bits_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/platform/filesystem_test.cpp"
               "${PROJECT_SOURCE_DIR}/inc/test/document_testing.hpp"
               "${PROJECT_SOURCE_DIR}/src/tile/animation_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/tile/tile_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/tile/tileset_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/ui/imgui_compat_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/ui/viewport_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/lookup_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/matrix_extent_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/matrix_index_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/scope_guard_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/string_conv_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/string_ops_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/util/uuid_test.cpp"
               "${PROJECT_SOURCE_DIR}/src/main.cpp"
               )

tactile_prepare_target(tactile-core-test)

target_include_directories(tactile-core-test PRIVATE "${PROJECT_SOURCE_DIR}/inc")

target_link_libraries(tactile-core-test
                      PUBLIC
                      tactile::core
                      GTest::gtest
                      )
