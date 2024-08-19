project(tactile-core-test CXX)

add_executable(tactile-core-test)

target_sources(tactile-core-test
               PRIVATE
               "src/cmd/layer/create_layer_command_test.cpp"
               "src/cmd/layer/remove_layer_command_test.cpp"
               "src/cmd/layer/move_layer_down_command_test.cpp"
               "src/cmd/layer/move_layer_up_command_test.cpp"
               "src/cmd/layer/set_layer_opacity_command_test.cpp"
               "src/cmd/layer/set_layer_visibility_command_test.cpp"
               "src/cmd/meta/create_property_command_test.cpp"
               "src/cmd/meta/remove_property_command_test.cpp"
               "src/cmd/meta/rename_property_command_test.cpp"
               "src/cmd/meta/update_property_command_test.cpp"
               "src/cmd/tile/add_tileset_command_test.cpp"
               "src/cmd/tile/remove_tileset_command_test.cpp"
               "src/cmd/command_stack_test.cpp"
               "src/debug/validation_test.cpp"
               "src/debug/validation_test.cpp"
               "src/document/layer_view_impl_test.cpp"
               "src/document/map_view_impl_test.cpp"
               "src/document/meta_view_impl_test.cpp"
               "src/document/tile_view_impl_test.cpp"
               "src/document/tileset_view_impl_test.cpp"
               "src/document/object_view_impl_test.cpp"
               "src/entity/registry_test.cpp"
               "src/event/event_dispatcher_test.cpp"
               "src/io/ini_test.cpp"
               "src/layer/group_layer_test.cpp"
               "src/layer/layer_common_test.cpp"
               "src/layer/layer_test.cpp"
               "src/layer/object_layer_test.cpp"
               "src/layer/object_test.cpp"
               "src/layer/tile_layer_test.cpp"
               "src/layer/tile_matrix_test.cpp"
               "src/map/map_spec_test.cpp"
               "src/map/map_test.cpp"
               "src/meta/attribute_type_test.cpp"
               "src/meta/color_test.cpp"
               "src/meta/meta_test.cpp"
               "src/model/settings_test.cpp"
               "src/numeric/random_test.cpp"
               "src/platform/filesystem_test.cpp"
               "src/tile/animation_test.cpp"
               "src/tile/tile_test.cpp"
               "src/tile/tileset_test.cpp"
               "src/ui/imgui_compat_test.cpp"
               "src/ui/viewport_test.cpp"
               "src/util/lookup_test.cpp"
               "src/util/scope_guard_test.cpp"
               "src/util/string_conv_test.cpp"
               "src/util/string_ops_test.cpp"
               "src/util/uuid_test.cpp"
               "src/ir_comparison.cpp"
               "src/main.cpp"

               PRIVATE FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/core/test/ir_comparison.hpp"
               "inc/test/document_testing.hpp"
               )

tactile_prepare_target(tactile-core-test)

target_link_libraries(tactile-core-test
                      PRIVATE
                      tactile::core
                      tactile::test_util
                      tactile::null_renderer
                      GTest::gtest
                      )
