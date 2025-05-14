project(tactile_core_project CXX)

find_package(Boost REQUIRED COMPONENTS safe_numerics)
find_package(FastFloat CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_library(tactile_core STATIC)

target_sources(tactile_core
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "common/common.cppm"
               "common/common-containers.cppm"
               "common/common-fs.cppm"
               "common/common-memory.cppm"
               "common/common-primitives.cppm"
               "common/common-result.cppm"
               "common/common-strings.cppm"
               "common/common-time.cppm"
               "io/io.cppm"
               "io/io-base64.cppm"
               "io/io-compression.cppm"
               "layer/layer.cppm"
               "layer/layer-annotation_layer.cppm"
               "layer/layer-group_layer.cppm"
               "layer/layer-interfaces.cppm"
               "layer/layer-layer_info.cppm"
               "layer/layer-tile_layer.cppm"
               "log/log.cppm"
               "log/log-buffer.cppm"
               "log/log-console_log_sink.cppm"
               "log/log-file_log_sink.cppm"
               "log/log-level.cppm"
               "log/log-logger.cppm"
               "log/log-sink.cppm"
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
               "tile/tile.cppm"
               "tile/tile-id.cppm"
               "util/util.cppm"
               "util/util-defer.cppm"
               "util/util-validation.cppm"
               "tactile_core.cppm"

               PRIVATE
               "common/result.cpp"
               "common/strings.cpp"
               "io/base64.cpp"
               "layer/annotation_layer.cpp"
               "layer/group_layer.cpp"
               "layer/layer_info.cpp"
               "layer/tile_layer.cpp"
               "log/console_log_sink.cpp"
               "log/file_log_sink.cpp"
               "log/logger.cpp"
               "meta/attr.cpp"
               "numeric/random.cpp"
               )

tactile_set_target_properties(tactile_core)

target_link_libraries(tactile_core
                      PRIVATE
                      FastFloat::fast_float

                      PUBLIC
                      tactile_interface_target
                      Boost::safe_numerics
                      )

target_include_directories(tactile_core
                           PRIVATE
                           "${CPPCODEC_INCLUDE_DIRS}"
                           )
