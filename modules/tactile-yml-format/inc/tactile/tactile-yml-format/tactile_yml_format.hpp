// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/io/save/save_format.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile {

/**
 * \brief Provides import and export support for the Tactile YAML format.
 *
 * \details Disabling "strict mode" when parsing with this format has the following effects.
 *          1. Properties may omit the otherwise required `value` attribute, and will assume the default
 *             values based on the `type` attribute instead in such cases.
 *          2. Images referenced by tilesets do not have to exist.
 *
 * \note This format uses ".yml" or ".yaml" file extensions.
 *
 * \see `docs/yaml-format.md` for more details.
 *
 * \todo Consistent strict mode.
 * \todo Embedded tilesets.
 * \todo Version handling.
 */
class TACTILE_YML_FORMAT_API TactileYmlFormat final : public ISaveFormat {
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

  [[nodiscard]]
  auto id() const -> SaveFormatId override;
};

}  // namespace tactile
