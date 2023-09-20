// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/format/file_format_manager.hpp"

#include "tactile/core/debug/profiler.hpp"
#include "tactile/core/format/file_format.hpp"

namespace tactile {

auto FileFormatManager::get() -> FileFormatManager&
{
  static FileFormatManager file_format_manager;
  return file_format_manager;
}

auto FileFormatManager::load_map(const fs::Path& map_file) const
    -> Expected<ir::Map, FileFormatError>
{
  const NativeStringView extension = map_file.extension().c_str();

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension)) {
      TACTILE_DEBUG_PROFILE_SCOPE("IFileFormat::load_map");
      return format->load_map(map_file);
    }
  }

  return unexpected(FileFormatError::kUnsupportedFormat);
}

auto FileFormatManager::load_tileset(const fs::Path& tileset_file) const
    -> Expected<ir::Tileset, FileFormatError>
{
  const NativeStringView extension = tileset_file.extension().c_str();

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension)) {
      TACTILE_DEBUG_PROFILE_SCOPE("IFileFormat::load_tileset");
      return format->load_tileset(tileset_file);
    }
  }

  return unexpected(FileFormatError::kUnsupportedFormat);
}

void FileFormatManager::add_format(IFileFormat* format)
{
  if (format) {
    mFormats.push_back(format);
  }
}

void FileFormatManager::remove_format(IFileFormat* format)
{
  std::erase(mFormats, format);
}

}  // namespace tactile
