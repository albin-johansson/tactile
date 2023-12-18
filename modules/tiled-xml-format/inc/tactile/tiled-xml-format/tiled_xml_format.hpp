// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/io/save/save_format.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile {

class TACTILE_TMX_FORMAT_API TiledXmlFormat final : public ISaveFormat {
 public:
  [[nodiscard]]
  auto load_map(const FilePath& map_path, const SaveFormatReadOptions& options) const
      -> Result<ir::Map> override;

  [[nodiscard]]
  auto load_tileset(const FilePath& tileset_path,
                    const SaveFormatReadOptions& options) const
      -> Result<ir::Tileset> override;

  [[nodiscard]]
  auto save_map(const FilePath& map_path,
                const ir::Map& map,
                const SaveFormatWriteOptions& options) const -> Result<void> override;

  [[nodiscard]]
  auto save_tileset(const FilePath& tileset_path,
                    const ir::Tileset& tileset,
                    const SaveFormatWriteOptions& options) const -> Result<void> override;

  [[nodiscard]]
  auto is_valid_extension(NativeStringView extension) const -> bool override;
};

}  // namespace tactile