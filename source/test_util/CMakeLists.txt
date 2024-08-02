project(tactile-test-util CXX)

add_library(tactile-test-util STATIC)
add_library(tactile::test_util ALIAS tactile-test-util)

target_sources(tactile-test-util
               PRIVATE
               "src/document_view_mocks.cpp"
               "src/ir.cpp"
               "src/ir_eq.cpp"
               "src/ir_presets.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/test_util/document_view_mocks.hpp"
               "inc/tactile/test_util/ir.hpp"
               "inc/tactile/test_util/ir_eq.hpp"
               "inc/tactile/test_util/ir_presets.hpp"
               )

tactile_prepare_target(tactile-test-util)

target_link_libraries(tactile-test-util
                      PUBLIC
                      tactile::base
                      GTest::gtest
                      GTest::gmock
                      )
