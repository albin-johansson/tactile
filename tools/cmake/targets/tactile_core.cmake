# Copyright (C) 2025 Albin Johansson

find_package(Boost REQUIRED COMPONENTS safe_numerics)
find_package(FastFloat CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

add_library(tactile_core STATIC)

tactile_set_target_properties(tactile_core)

target_sources(tactile_core
               PUBLIC FILE_SET CXX_MODULES FILES
               "${TACTILE_MODULE_DIR}/tactile/core/base64/base64.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-containers.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-fs.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-memory.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-platform.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-primitives.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-result.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-strings.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/common/common-time.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/ext/boost/safe_numerics/safe_numerics.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/ext/cppcodec/cppcodec.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/ext/std/std.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/io/io.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/io/io-compression.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/layer/layer.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/layer/layer-annotation_layer.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/layer/layer-group_layer.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/layer/layer-interfaces.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/layer/layer-layer_info.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/layer/layer-tile_layer.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/log/log.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/log/log-buffer.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/log/log-console_log_sink.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/log/log-file_log_sink.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/log/log-level.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/log/log-logger.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/log/log-sink.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/meta/meta.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/meta/meta-attr.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/meta/meta-color.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/numeric/numeric.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/numeric/numeric-checked.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/numeric/numeric-concepts.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/numeric/numeric-vec.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/runtime/runtime.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/runtime/runtime-interfaces.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/runtime/runtime-null.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/save/save.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/serdes/serdes.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/tile/tile.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/util/util.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/util/util-defer.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/util/util-validation.cppm"
               "${TACTILE_MODULE_DIR}/tactile/core/core.cppm"

               PRIVATE
               "${TACTILE_SOURCE_DIR}/tactile/core/base64/base64.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/common/result.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/io/compression.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/layer/annotation_layer.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/layer/group_layer.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/layer/layer_info.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/layer/tile_layer.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/log/console_log_sink.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/log/file_log_sink.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/log/logger.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/meta/attr.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/numeric/random.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/runtime/runtime.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/serdes/serdes.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/tile/tile.cpp"
               "${TACTILE_SOURCE_DIR}/tactile/core/tile/tile_animation.cpp"
               )

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
