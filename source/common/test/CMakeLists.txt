project("tactile-common-test-project" CXX)

add_executable("tactile-common-test")

tactile_prepare_target("tactile-common-test")

target_sources("tactile-common-test"
               PRIVATE
               "src/container/buffer_test.cpp"
               "src/container/lookup_test.cpp"
               "src/container/string_utils_test.cpp"
               "src/io/format_test.cpp"
               "src/meta/colors_test.cpp"
               "src/numeric/charconv_test.cpp"
               "src/numeric/random_test.cpp"
               "src/serdes/tiles_test.cpp"
               "src/main.cpp"
               )

target_link_libraries("tactile-common-test"
                      PRIVATE
                      "GTest::gtest"
                      "tactile::common"
                      )
