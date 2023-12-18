// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/save/save_format_context.hpp"

#include <filesystem>  // create_directories

#include <fmt/ostream.h>

#include "tactile/foundation/debug/profiler.hpp"
#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

auto SaveFormatContext::get() noexcept -> SaveFormatContext&
{
  static SaveFormatContext save_format_manager;
  return save_format_manager;
}

auto SaveFormatContext::load_map(const FilePath& map_path,
                                 const SaveFormatReadOptions& options) const
    -> Result<ir::Map>
{
  TACTILE_LOG_INFO("Loading map from {}", fmt::streamed(map_path));
  const auto extension = map_path.extension();

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension.c_str())) {
      TACTILE_DEBUG_PROFILE_SCOPE("SaveFormatContext::load_map");
      if (auto map = format->load_map(map_path, options)) {
        return map;
      }
      else {
        TACTILE_LOG_ERROR("Could not load map: {}", map.error().message());
        return propagate_unexpected(map);
      }
    }
  }

  TACTILE_LOG_ERROR("Tried to load map with unsupported format");
  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedFormat));
}

auto SaveFormatContext::load_tileset(const FilePath& tileset_path,
                                     const SaveFormatReadOptions& options) const
    -> Result<ir::Tileset>
{
  TACTILE_LOG_INFO("Loading tileset from {}", fmt::streamed(tileset_path));
  const auto extension = tileset_path.extension();

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension.c_str())) {
      TACTILE_DEBUG_PROFILE_SCOPE("SaveFormatContext::load_tileset");
      if (auto tileset = format->load_tileset(tileset_path, options)) {
        return tileset;
      }
      else {
        TACTILE_LOG_ERROR("Could not load tileset: {}", tileset.error().message());
        return propagate_unexpected(tileset);
      }
    }
  }

  TACTILE_LOG_ERROR("Tried to load tileset with unsupported format");
  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedFormat));
}

auto SaveFormatContext::save_map(const FilePath& map_path,
                                 const ir::Map& map,
                                 const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  TACTILE_LOG_INFO("Saving map to {}", fmt::streamed(map_path));

  std::filesystem::create_directories(map_path.parent_path());
  const auto extension = map_path.extension();

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension.c_str())) {
      TACTILE_DEBUG_PROFILE_SCOPE("SaveFormatContext::save_map");
      if (auto result = format->save_map(map_path, map, options)) {
        return result;
      }
      else {
        TACTILE_LOG_ERROR("Could not save map: {}", result.error().message());
        return propagate_unexpected(result);
      }
    }
  }

  TACTILE_LOG_ERROR("Tried to save map with unsupported format");
  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedFormat));
}

auto SaveFormatContext::save_tileset(const FilePath& tileset_path,
                                     const ir::Tileset& tileset,
                                     const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  TACTILE_LOG_INFO("Saving tileset to {}", fmt::streamed(tileset_path));

  std::filesystem::create_directories(tileset_path.parent_path());
  const auto extension = tileset_path.extension();

  for (const auto* format : mFormats) {
    if (format->is_valid_extension(extension.c_str())) {
      TACTILE_DEBUG_PROFILE_SCOPE("SaveFormatContext::save_tileset");
      if (auto result = format->save_tileset(tileset_path, tileset, options)) {
        return result;
      }
      else {
        TACTILE_LOG_ERROR("Could not save tileset: {}", result.error().message());
        return propagate_unexpected(result);
      }
    }
  }

  TACTILE_LOG_ERROR("Tried to save tileset with unsupported format");
  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedFormat));
}

void SaveFormatContext::add_format(ISaveFormat* format)
{
  if (format) {
    mFormats.push_back(format);
  }
}

void SaveFormatContext::remove_format(ISaveFormat* format) noexcept
{
  std::erase(mFormats, format);
}

}  // namespace tactile