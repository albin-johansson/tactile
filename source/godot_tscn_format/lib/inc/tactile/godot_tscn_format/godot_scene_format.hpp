// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/runtime.hpp"
#include "tactile/godot_tscn_format/api.hpp"

namespace tactile::godot {

/**
 * Provides support for the scene format used by the Godot game engine.
 *
 * \see https://docs.godotengine.org/en/stable/contributing/development/file_formats/tscn.html
 * \see https://docs.godotengine.org/en/3.6/development/file_formats/tscn.html
 */
class TACTILE_GODOT_API GodotSceneFormat final : public ISaveFormat
{
 public:
  explicit GodotSceneFormat(IRuntime* runtime);

  [[nodiscard]]
  auto load_map(const std::filesystem::path& map_path,
                const SaveFormatReadOptions& options) const
      -> std::expected<ir::Map, std::error_code> override;

  [[nodiscard]]
  auto save_map(const IMapView& map, const SaveFormatWriteOptions& options) const
      -> std::expected<void, std::error_code> override;

 private:
  IRuntime* m_runtime;
};

}  // namespace tactile::godot
