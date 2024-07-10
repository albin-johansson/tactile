project(tactile-base CXX)

add_library(tactile-base INTERFACE)
add_library(tactile::base ALIAS tactile-base)

target_sources(tactile-base
               INTERFACE
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/array.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/deque.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/expected.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/function.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/hash_map.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/maybe.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/pair.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/path.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/queue.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/ref.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/set.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/smart_ptr.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/span.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/string.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/string_map.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/tree_map.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/variant.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/container/vector.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/engine/engine_app.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/io/compress/compression_format.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/io/compress/compressor.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/io/save/save_format_id.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/io/byte_stream.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/layer/layer_type.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/layer/object_type.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/layer/tile_encoding.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/layer/tile_orientation.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/log/log_level.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/meta/attribute.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/meta/attribute_type.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/meta/color.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/numeric/saturate_cast.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/numeric/sign_cast.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/numeric/vec.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/numeric/vec_common.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/numeric/vec_format.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/numeric/vec_stream.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/render/renderer.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/render/texture.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/render/window.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/util/buffer.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/util/chrono.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/util/concepts.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/util/format.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/util/hash.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/util/strong_type.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/id.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/int.hpp"
               "${PROJECT_SOURCE_DIR}/inc/tactile/base/prelude.hpp"
               )

target_include_directories(tactile-base
                           INTERFACE
                           "${PROJECT_SOURCE_DIR}/inc"
                           )

if (TACTILE_BUILD_TESTS)
  add_subdirectory("test")
endif ()
