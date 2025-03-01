project(tactile-core-lib CXX)

find_package(boost_container CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(tactile_core SHARED)
add_library(tactile::core ALIAS tactile_core)

tactile_prepare_target(tactile_core)

target_sources(tactile_core
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/core/basic/array.hpp"
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
               "inc/tactile/core/log/console_log_sink.hpp"
               "inc/tactile/core/log/log_level.hpp"
               "inc/tactile/core/log/log_sink.hpp"
               "inc/tactile/core/log/logger.hpp"
               "inc/tactile/core/math/hash.hpp"
               "inc/tactile/core/math/vec.hpp"
               "inc/tactile/core/misc/bounds.hpp"
               "inc/tactile/core/misc/name.hpp"
               "inc/tactile/core/misc/defer.hpp"
               "inc/tactile/core/tile/tile_animation.hpp"
               "inc/tactile/core/tile/tile_id.hpp"
               "inc/tactile/core/tile/tile_index.hpp"
               "inc/tactile/core/api.hpp"
               "inc/tactile/core/fwd.hpp"
               "inc/tactile/core/world.hpp"

               PRIVATE FILE_SET "private_headers" TYPE "HEADERS" BASE_DIRS "src" FILES
               "src/tactile/core/world_data.hpp"

               PRIVATE
               "src/tactile/core/basic/errc.cpp"
               "src/tactile/core/log/console_log_sink.cpp"
               "src/tactile/core/log/logger.cpp"
               "src/tactile/core/tile/tile_animation.cpp"
               "src/tactile/core/world.cpp"
               "src/tactile/core/world_data.cpp"
               )

target_link_libraries(tactile_core
                      PRIVATE
                      tactile_basic_target
                      Boost::container
                      EnTT::EnTT

                      PUBLIC
                      fmt::fmt
                      )

target_compile_definitions(tactile_core
                           PRIVATE
                           TACTILE_BUILDING_CORE
                           )
