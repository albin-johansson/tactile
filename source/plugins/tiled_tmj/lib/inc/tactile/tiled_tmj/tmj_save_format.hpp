// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj/api.hpp"

namespace tactile {

class IRuntime;

/**
 * Implements the Tiled TMJ save format.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/json-map-format/
 */
class TACTILE_TMJ_FORMAT_API TmjSaveFormat final : public ISaveFormat
{
 public:
  TmjSaveFormat(IRuntime* runtime);

  [[nodiscard]]
  auto load_map(const std::filesystem::path& map_path, const SaveFormatReadOptions& options)
      const -> std::expected<ir::Map, std::error_code> override;

  [[nodiscard]]
  auto save_map(const IMapView& map, const SaveFormatWriteOptions& options) const
      -> std::expected<void, std::error_code> override;

 private:
  IRuntime* mRuntime;
};

}  // namespace tactile
