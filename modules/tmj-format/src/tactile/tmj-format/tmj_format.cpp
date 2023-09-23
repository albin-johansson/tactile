// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/tmj_format.hpp"

#include "tactile/core/debug/log/logger.hpp"

namespace tactile {

auto TmjFormat::load_map(const fs::Path& map_file) const
    -> Expected<ir::Map, FileFormatError>
{
  try {
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Error reading TMJ map: {}", ex.what());
    return unexpected(FileFormatError::kUnknown);
  }

  TACTILE_LOG_ERROR("An unknown error occurred during TMJ parsing");
  return unexpected(FileFormatError::kUnknown);
}

auto TmjFormat::load_tileset(const fs::Path& tileset_file) const
    -> Expected<ir::Tileset, FileFormatError>
{
  return unexpected(FileFormatError::kUnknown);
}

void TmjFormat::save_map(const fs::Path& map_file, const ir::Map& map)
{
  // TODO
}

void TmjFormat::save_tileset(const fs::Path& tileset_file, const ir::Tileset& tileset)
{
  // TODO
}

auto TmjFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".tmj") ||
         extension == TACTILE_NATIVE_STR(".json");
}

}  // namespace tactile