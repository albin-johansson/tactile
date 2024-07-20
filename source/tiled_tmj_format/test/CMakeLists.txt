project(tactile-tiled-tmj-format-test CXX)

add_executable(tactile-tmj-format-test)

target_sources(tactile-tmj-format-test
               PRIVATE
               "src/main.cpp"
               "src/tmj_format_attribute_parser_test.cpp"
               )

tactile_prepare_target(tactile-tmj-format-test)

target_link_libraries(tactile-tmj-format-test
                      PRIVATE
                      tactile::tiled_tmj_format
                      tactile::null_renderer
                      GTest::gtest
                      )
