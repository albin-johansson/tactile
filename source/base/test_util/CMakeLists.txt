project(tactile-base-test-util CXX)

add_library(tactile-base-test-util STATIC)
add_library(tactile::base_test_util ALIAS tactile-base-test-util)

target_sources(tactile-base-test-util
               PRIVATE
               "src/ir.cpp"

               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/base/test_util/ir.hpp"
               )

tactile_prepare_target(tactile-base-test-util)

target_link_libraries(tactile-base-test-util
                      PUBLIC
                      tactile::base
                      )
