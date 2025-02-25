project(tactile-core-lib CXX)

add_library(tactile_core SHARED)
add_library(tactile::core ALIAS tactile_core)

tactile_prepare_target(tactile_core)

target_sources(tactile_core
               PUBLIC FILE_SET HEADERS BASE_DIRS inc FILES
               "inc/tactile/core/basic/errc.hpp"
               "inc/tactile/core/basic/option.hpp"
               "inc/tactile/core/basic/result.hpp"
               "inc/tactile/core/basic/string.hpp"
               "inc/tactile/core/basic/vec.hpp"

               PRIVATE
               "src/tactile/core/basic/errc.cpp"
               )

target_link_libraries(tactile_core
                      PRIVATE
                      tactile_basic_target
                      )
