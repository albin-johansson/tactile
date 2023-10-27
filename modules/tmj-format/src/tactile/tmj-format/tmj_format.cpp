// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/tmj_format.hpp"

#include <exception>  // exception

#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/io/save/save_format_error.hpp"
#include "tactile/core/io/stream.hpp"
#include "tactile/tmj-format/common/json.hpp"
#include "tactile/tmj-format/emit/tmj_map_emitter.hpp"
#include "tactile/tmj-format/emit/tmj_tileset_emitter.hpp"
#include "tactile/tmj-format/load/tmj_map_parser.hpp"

namespace tactile::tmj {

auto TmjFormat::load_map(const FilePath& map_path,
                         const SaveFormatReadOptions& options) const -> Result<ir::Map>
{
  try {
    if (auto parsed_json = parse_json(map_path)) {
      return parse_map(*parsed_json, map_path.filename().string(), options);
    }
    else {
      return propagate_unexpected(parsed_json);
    }
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("[TMJ] Error reading map: {}", ex.what());
    return error(SaveFormatError::kUnknown);
  }
  catch (...) {
    TACTILE_LOG_ERROR("[TMJ] An unknown error occurred during parsing");
    return error(SaveFormatError::kUnknown);
  }
}

auto TmjFormat::load_tileset(const FilePath& tileset_path,
                             const SaveFormatReadOptions& options) const
    -> Result<ir::Tileset>
{
  return error(SaveFormatError::kUnknown);
}

auto TmjFormat::save_map(const FilePath& map_path,
                         const ir::Map& map,
                         const SaveFormatWriteOptions& options) const -> Result<void>
{
  if (const auto map_json = emit_map(map, options)) {
    const StreamToFileOptions stream_options {
      .indentation = options.use_indentation ? 2 : 0,
      .binary_mode = false,
    };

    stream_to_file(*map_json, map_path, stream_options);
    return kSuccess;
  }
  else {
    return propagate_unexpected(map_json);
  }
}

auto TmjFormat::save_tileset(const FilePath& tileset_path,
                             const ir::Tileset& tileset,
                             const SaveFormatWriteOptions& options) const -> Result<void>
{
  // TODO
  return kSuccess;
}

auto TmjFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".tmj") ||
         extension == TACTILE_NATIVE_STR(".json");
}

}  // namespace tactile::tmj