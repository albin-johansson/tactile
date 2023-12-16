// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/tiled_json_format.hpp"

#include <exception>  // exception

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/io/stream.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tiled-json-format/common.hpp"
#include "tactile/tiled-json-format/emit/map_emitter.hpp"
#include "tactile/tiled-json-format/emit/tileset_emitter.hpp"
#include "tactile/tiled-json-format/parse/map_parser.hpp"

namespace tactile {

auto TiledJsonFormat::load_map(const FilePath& map_path,
                               const SaveFormatReadOptions& options) const
    -> Result<ir::Map>
{
  try {
    return tiled::tmj::parse_json(map_path).and_then([&](const tiled::tmj::JSON& json) {
      return tiled::tmj::parse_map(json, map_path.filename().string(), options);
    });
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("[TMJ] Error reading map: {}", ex.what());
    return unexpected(make_save_format_error(SaveFormatError::kUnknown));
  }
  catch (...) {
    TACTILE_LOG_ERROR("[TMJ] An unknown error occurred during parsing");
    return unexpected(make_save_format_error(SaveFormatError::kUnknown));
  }
}

auto TiledJsonFormat::load_tileset(const FilePath& tileset_path,
                                   const SaveFormatReadOptions& options) const
    -> Result<ir::Tileset>
{
  // TODO
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TiledJsonFormat::save_map(const FilePath& map_path,
                               const ir::Map& map,
                               const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  return tiled::tmj::emit_map(map, options)
      .and_then([&](const tiled::tmj::JSON& map_json) {
        const StreamToFileOptions stream_options {
          .indentation = options.use_indentation ? 2 : 0,
          .binary_mode = false,
        };

        return stream_to_file(map_json, map_path, stream_options);
      });
}

auto TiledJsonFormat::save_tileset(const FilePath& tileset_path,
                                   const ir::Tileset& tileset,
                                   const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  // TODO
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TiledJsonFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".tmj") ||
         extension == TACTILE_NATIVE_STR(".json");
}

}  // namespace tactile