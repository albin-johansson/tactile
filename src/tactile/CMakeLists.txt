project(tactile_module CXX)

find_package(Boost REQUIRED COMPONENTS safe_numerics)
find_package(FastFloat CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_library(tactile STATIC)

target_sources(tactile
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "core/core.cppm"
               "core/core-containers.cppm"
               "core/core-fs.cppm"
               "core/core-memory.cppm"
               "core/core-primitives.cppm"
               "core/core-result.cppm"
               "core/core-strings.cppm"
               "core/core-time.cppm"
               "io/io.cppm"
               "io/io-base64.cppm"
               "io/io-compression.cppm"
               "layer/layer.cppm"
               "layer/layer-annotation_layer.cppm"
               "layer/layer-group_layer.cppm"
               "layer/layer-interfaces.cppm"
               "layer/layer-layer_info.cppm"
               "layer/layer-tile_layer.cppm"
               "meta/meta.cppm"
               "meta/meta-attr.cppm"
               "meta/meta-color.cppm"
               "numeric/numeric.cppm"
               "numeric/numeric-casts.cppm"
               "numeric/numeric-checked.cppm"
               "numeric/numeric-concepts.cppm"
               "numeric/numeric-constants.cppm"
               "numeric/numeric-hash.cppm"
               "numeric/numeric-random.cppm"
               "numeric/numeric-vec.cppm"
               "tactile.cppm"

               PRIVATE
               "core/result.cpp"
               "core/strings.cpp"
               "io/base64.cpp"
               "layer/annotation_layer.cpp"
               "layer/group_layer.cpp"
               "layer/layer_info.cpp"
               "layer/tile_layer.cpp"
               "meta/attr.cpp"
               "numeric/random.cpp"
               )

tactile_prepare_target(tactile)

target_link_libraries(tactile
                      PRIVATE
                      FastFloat::fast_float

                      PUBLIC
                      Boost::safe_numerics
                      )

target_include_directories(tactile
                           PRIVATE
                           "${CPPCODEC_INCLUDE_DIRS}"
                           )
