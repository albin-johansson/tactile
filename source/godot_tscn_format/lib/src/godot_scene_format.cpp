// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn_format/godot_scene_format.hpp"

#include <system_error>  // make_error_code, errc

namespace tactile::godot {

GodotSceneFormat::GodotSceneFormat(IRuntime* runtime)
  : m_runtime {runtime}
{}

auto GodotSceneFormat::load_map(const std::filesystem::path& map_path,
                                const SaveFormatReadOptions& options) const
    -> std::expected<ir::Map, std::error_code>
{
  return std::unexpected {std::make_error_code(std::errc::not_supported)};
}

auto GodotSceneFormat::save_map(const IMapView& map,
                                const SaveFormatWriteOptions& options) const
    -> std::expected<void, std::error_code>
{
  return std::unexpected {std::make_error_code(std::errc::not_supported)};
}

}  // namespace tactile::godot
