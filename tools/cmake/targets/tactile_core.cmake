# Copyright (C) 2025 Albin Johansson

find_package(Boost REQUIRED COMPONENTS safe_numerics)
find_package(FastFloat CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_library(tactile_core STATIC)

target_sources(tactile_core
               PUBLIC FILE_SET "CXX_MODULES" FILES
               "${TACTILE_SOURCE_DIR}/core/common/common.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-containers.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-fs.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-memory.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-platform.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-primitives.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-result.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-strings.cppm"
               "${TACTILE_SOURCE_DIR}/core/common/common-time.cppm"
               "${TACTILE_SOURCE_DIR}/core/io/io.cppm"
               "${TACTILE_SOURCE_DIR}/core/io/io-base64.cppm"
               "${TACTILE_SOURCE_DIR}/core/io/io-compression.cppm"
               "${TACTILE_SOURCE_DIR}/core/layer/layer.cppm"
               "${TACTILE_SOURCE_DIR}/core/layer/layer-annotation_layer.cppm"
               "${TACTILE_SOURCE_DIR}/core/layer/layer-group_layer.cppm"
               "${TACTILE_SOURCE_DIR}/core/layer/layer-interfaces.cppm"
               "${TACTILE_SOURCE_DIR}/core/layer/layer-layer_info.cppm"
               "${TACTILE_SOURCE_DIR}/core/layer/layer-tile_layer.cppm"
               "${TACTILE_SOURCE_DIR}/core/log/log.cppm"
               "${TACTILE_SOURCE_DIR}/core/log/log-buffer.cppm"
               "${TACTILE_SOURCE_DIR}/core/log/log-console_log_sink.cppm"
               "${TACTILE_SOURCE_DIR}/core/log/log-file_log_sink.cppm"
               "${TACTILE_SOURCE_DIR}/core/log/log-level.cppm"
               "${TACTILE_SOURCE_DIR}/core/log/log-logger.cppm"
               "${TACTILE_SOURCE_DIR}/core/log/log-sink.cppm"
               "${TACTILE_SOURCE_DIR}/core/meta/meta.cppm"
               "${TACTILE_SOURCE_DIR}/core/meta/meta-attr.cppm"
               "${TACTILE_SOURCE_DIR}/core/meta/meta-color.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric-casts.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric-checked.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric-concepts.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric-constants.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric-hash.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric-random.cppm"
               "${TACTILE_SOURCE_DIR}/core/numeric/numeric-vec.cppm"
               "${TACTILE_SOURCE_DIR}/core/runtime/runtime.cppm"
               "${TACTILE_SOURCE_DIR}/core/runtime/runtime-interfaces.cppm"
               "${TACTILE_SOURCE_DIR}/core/runtime/runtime-null.cppm"
               "${TACTILE_SOURCE_DIR}/core/save/save.cppm"
               "${TACTILE_SOURCE_DIR}/core/tile/tile.cppm"
               "${TACTILE_SOURCE_DIR}/core/util/util.cppm"
               "${TACTILE_SOURCE_DIR}/core/util/util-defer.cppm"
               "${TACTILE_SOURCE_DIR}/core/util/util-validation.cppm"
               "${TACTILE_SOURCE_DIR}/core/core.cppm"

               PRIVATE
               "${TACTILE_SOURCE_DIR}/core/common/result.cpp"
               "${TACTILE_SOURCE_DIR}/core/common/strings.cpp"
               "${TACTILE_SOURCE_DIR}/core/io/base64.cpp"
               "${TACTILE_SOURCE_DIR}/core/io/compression.cpp"
               "${TACTILE_SOURCE_DIR}/core/layer/annotation_layer.cpp"
               "${TACTILE_SOURCE_DIR}/core/layer/group_layer.cpp"
               "${TACTILE_SOURCE_DIR}/core/layer/layer_info.cpp"
               "${TACTILE_SOURCE_DIR}/core/layer/tile_layer.cpp"
               "${TACTILE_SOURCE_DIR}/core/log/console_log_sink.cpp"
               "${TACTILE_SOURCE_DIR}/core/log/file_log_sink.cpp"
               "${TACTILE_SOURCE_DIR}/core/log/logger.cpp"
               "${TACTILE_SOURCE_DIR}/core/meta/attr.cpp"
               "${TACTILE_SOURCE_DIR}/core/numeric/random.cpp"
               "${TACTILE_SOURCE_DIR}/core/runtime/runtime.cpp"
               "${TACTILE_SOURCE_DIR}/core/tile/tile.cpp"
               "${TACTILE_SOURCE_DIR}/core/tile/tile_animation.cpp"
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
