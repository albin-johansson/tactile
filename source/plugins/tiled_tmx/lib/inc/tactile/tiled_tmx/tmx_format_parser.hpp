#pragma once

#include <expected>    // expected
#include <filesystem>  // path

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/runtime/runtime.hpp"
#include "tactile/tiled_tmx/api.hpp"

namespace tactile::tiled_tmx {

/**
 * Attempts to parse a single Tiled TMX map.
 *
 * \param runtime  The associated runtime.
 * \param map_path The file path to the TMX map.
 * \param options  The configured read options.
 *
 * \return
 * The parsed map if successful; an error code otherwise.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#map
 */
[[nodiscard]]
TACTILE_TILED_TMX_API auto parse_map(const IRuntime& runtime,
                                     const std::filesystem::path& map_path,
                                     const SaveFormatReadOptions& options)
    -> std::expected<ir::Map, ErrorCode>;

}  // namespace tactile::tiled_tmx
