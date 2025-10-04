# Copyright (C) 2025 Albin Johansson
# This software is provided under the terms of the MIT License.

add_library(tactile_core STATIC)

tactile_init_target(tactile_core)

target_sources(tactile_core
               PUBLIC FILE_SET "HEADERS" BASE_DIRS "${PROJECT_SOURCE_DIR}/include" FILES
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/hash_map.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/option.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/smart_ptr.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/span.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/string.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/variant.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/container/vector.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/concepts.hpp"
               "${PROJECT_SOURCE_DIR}/include/tactile/core/primitives.hpp"
               )
