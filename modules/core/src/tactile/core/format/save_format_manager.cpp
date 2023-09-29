// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/format/save_format_manager.hpp"

#include "tactile/core/debug/profiler.hpp"
#include "tactile/core/format/save_format.hpp"
#include "tactile/core/format/save_format_error.hpp"

namespace tactile {

auto SaveFormatManager::get() -> SaveFormatManager&
{
  static SaveFormatManager file_format_manager;
  return file_format_manager;
}

auto SaveFormatManager::load_map(const fs::Path& map_file) const -> Result<ir::Map>
{
  const NativeStringView extension = map_file.extension().c_str();

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension)) {
      TACTILE_DEBUG_PROFILE_SCOPE("ISaveFormat::load_map");
      return format->load_map(map_file);
    }
  }

  return unexpected(save_format_error(SaveFormatError::kUnsupportedFormat));
}

auto SaveFormatManager::load_tileset(const fs::Path& tileset_file) const
    -> Result<ir::Tileset>
{
  const NativeStringView extension {tileset_file.extension().c_str()};

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension)) {
      TACTILE_DEBUG_PROFILE_SCOPE("ISaveFormat::load_tileset");
      return format->load_tileset(tileset_file);
    }
  }

  return unexpected(save_format_error(SaveFormatError::kUnsupportedFormat));
}

void SaveFormatManager::add_format(ISaveFormat* format)
{
  if (format) {
    mFormats.push_back(format);
  }
}

void SaveFormatManager::remove_format(ISaveFormat* format)
{
  std::erase(mFormats, format);
}

}  // namespace tactile
