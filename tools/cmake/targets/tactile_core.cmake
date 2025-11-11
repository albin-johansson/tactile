# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

add_library(tactile_core STATIC)

tactile_init_target(tactile_core)

target_sources(tactile_core
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "${PROJECT_SOURCE_DIR}/include" FILES
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/array.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/deque.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/function.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/hash_map.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/lookup.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/option.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/path.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/smart_ptr.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/span.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/string.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/tree_map.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/variant.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/vector.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/layer/layer.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/layer/layer_id.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/log/console_log_sink.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/log/file_log_sink.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/log/log_category.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/log/log_entry.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/log/log_sink.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/log/logger.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/math/vec.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/meta/attribute.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/meta/attribute_set.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/meta/color.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/meta/opacity.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/tile/tile_id.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/util/defer.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/util/time.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/util/format_buffer.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/casts.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/concepts.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/fwd.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/macros.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/primitives.hpp"

               PRIVATE
               "${PROJECT_SOURCE_DIR}/src/tactile/core/layer/layer.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/core/log/console_log_sink.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/core/log/file_log_sink.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/core/log/logger.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/core/meta/attribute.cpp"
               "${PROJECT_SOURCE_DIR}/src/tactile/core/meta/attribute_set.cpp"
               )
