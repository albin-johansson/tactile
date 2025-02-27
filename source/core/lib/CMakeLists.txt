project(tactile-core-lib CXX)

add_library(tactile_core SHARED)
add_library(tactile::core ALIAS tactile_core)

tactile_prepare_target(tactile_core)

target_sources(tactile_core
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/core/basic/casts.hpp"
               "inc/tactile/core/basic/clock.hpp"
               "inc/tactile/core/basic/errc.hpp"
               "inc/tactile/core/basic/hash_map.hpp"
               "inc/tactile/core/basic/macros.hpp"
               "inc/tactile/core/basic/option.hpp"
               "inc/tactile/core/basic/primitives.hpp"
               "inc/tactile/core/basic/result.hpp"
               "inc/tactile/core/basic/smart_ptr.hpp"
               "inc/tactile/core/basic/string.hpp"
               "inc/tactile/core/basic/vector.hpp"
               "inc/tactile/core/entity/entity_id.hpp"
               "inc/tactile/core/tile/tile_id.hpp"

               PRIVATE
               "src/tactile/core/basic/errc.cpp"
               )

target_link_libraries(tactile_core
                      PRIVATE
                      tactile_basic_target
                      )

target_compile_definitions(tactile_core
                           PRIVATE
                           TACTILE_BUILDING_CORE
                           )
