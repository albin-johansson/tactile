#pragma once

#include "tactile/base/io/save/save_format.hpp"
#include "tactile/base/runtime/runtime.hpp"
#include "tactile/tiled_tmx/api.hpp"

namespace tactile::tiled_tmx {

/**
 * Implements the Tiled TMX save format.
 *
 * \see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/
 */
class TACTILE_TILED_TMX_API TmxSaveFormat final : public ISaveFormat
{
 public:
  explicit TmxSaveFormat(IRuntime* runtime);

  [[nodiscard]]
  auto load_map(const std::filesystem::path& map_path,
                const SaveFormatReadOptions& options) const
      -> std::expected<ir::Map, ErrorCode> override;

  [[nodiscard]]
  auto save_map(const IMapView& map, const SaveFormatWriteOptions& options) const
      -> std::expected<void, ErrorCode> override;

 private:
  IRuntime* m_runtime;
};

}  // namespace tactile::tiled_tmx
