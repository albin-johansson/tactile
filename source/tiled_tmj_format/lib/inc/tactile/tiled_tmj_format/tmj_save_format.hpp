// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/tiled_tmj_format/api.hpp"

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
  auto load_map(const Path& map_path,
                const SaveFormatReadOptions& options) const -> Result<ir::Map> override;

  [[nodiscard]]
  auto save_map(const IMapView& map,
                const SaveFormatWriteOptions& options) const -> Result<void> override;

 private:
  IRuntime* mRuntime;
};

}  // namespace tactile
