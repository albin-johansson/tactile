project("tactile-common-lib-project" CXX)

add_library("tactile-common" STATIC)
add_library("tactile::common" ALIAS "tactile-common")

tactile_prepare_target("tactile-common")

target_sources("tactile-common"
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "inc" FILES
               "inc/tactile/common/container/buffer.hpp"
               "inc/tactile/common/container/lookup.hpp"
               "inc/tactile/common/container/string_map.hpp"
               "inc/tactile/common/container/string_utils.hpp"
               "inc/tactile/common/debug/validation.hpp"
               "inc/tactile/common/io/file_streams.hpp"
               "inc/tactile/common/io/format.hpp"
               "inc/tactile/common/meta/colors.hpp"
               "inc/tactile/common/numeric/charconv.hpp"
               "inc/tactile/common/numeric/random.hpp"
               "inc/tactile/common/serdes/tiles.hpp"

               PRIVATE
               "src/io/file_streams.cpp"
               "src/meta/colors.cpp"
               "src/numeric/charconv.cpp"
               "src/numeric/random.cpp"
               "src/serdes/tiles.cpp"
               )

target_link_libraries("tactile-common"
                      PUBLIC
                      "tactile-base"

                      PRIVATE
                      "FastFloat::fast_float"
                      )
