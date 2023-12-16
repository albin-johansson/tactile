// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot-scene-format/godot_scene_format.hpp"

#include "tactile/foundation/debug/generic_error.hpp"

namespace tactile {

auto GodotSceneFormat::load_map(const FilePath&, const SaveFormatReadOptions&) const
    -> Result<ir::Map>
{
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto GodotSceneFormat::load_tileset(const FilePath&, const SaveFormatReadOptions&) const
    -> Result<ir::Tileset>
{
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto GodotSceneFormat::save_map(const FilePath& map_path,
                                const ir::Map& map,
                                const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  (void) map_path;
  (void) map;
  (void) options;
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto GodotSceneFormat::save_tileset(const FilePath& tileset_path,
                                    const ir::Tileset& tileset,
                                    const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  (void) tileset_path;
  (void) tileset;
  (void) options;
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto GodotSceneFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".tscn") ||
         extension == TACTILE_NATIVE_STR(".escn");
}

}  // namespace tactile
