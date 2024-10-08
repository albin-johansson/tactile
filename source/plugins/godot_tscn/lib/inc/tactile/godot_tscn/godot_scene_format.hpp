// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/godot_tscn/api.hpp"

namespace tactile::godot_tscn {

/**
 * Provides support for the scene format used by the Godot game engine.
 *
 * \details
 * This save format supports the following custom settings.
 * - \c "version": An integer indicating which major version of Godot to target.
 * - \c "ellipse_polygon_vertices": An integer indicating the number of vertices to use when
 *                                  approximating ellipse objects as polygons.
 *
 * \see https://docs.godotengine.org/en/stable/contributing/development/file_formats/tscn.html
 * \see https://docs.godotengine.org/en/3.6/development/file_formats/tscn.html
 */
class TACTILE_GODOT_API GodotSceneFormat final : public ISaveFormat
{
 public:
  [[nodiscard]]
  auto load_map(const std::filesystem::path& map_path,
                const SaveFormatReadOptions& options) const
      -> std::expected<ir::Map, std::error_code> override;

  [[nodiscard]]
  auto save_map(const IMapView& map, const SaveFormatWriteOptions& options) const
      -> std::expected<void, std::error_code> override;
};

}  // namespace tactile::godot_tscn
