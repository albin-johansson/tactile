// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/document/tileset_view.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/document/meta_view_impl.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class MapDocument;

/**
 * A tileset view implementation.
 */
class TilesetViewImpl final : public ITilesetView
{
 public:
  /**
   * Creates a view of a tileset used in a map document.
   *
   * \param document   The associated map document.
   * \param tileset_id The tileset entity identifier.
   */
  TilesetViewImpl(const MapDocument* document, EntityID tileset_id);

  [[nodiscard]]
  auto accept(IDocumentVisitor& visitor) const -> std::expected<void, ErrorCode> override;

  [[nodiscard]]
  auto get_first_tile_id() const -> TileID override;

  [[nodiscard]]
  auto tile_count() const -> std::size_t override;

  [[nodiscard]]
  auto tile_definition_count() const -> std::size_t override;

  [[nodiscard]]
  auto column_count() const -> std::size_t override;

  [[nodiscard]]
  auto get_tile_size() const -> Int2 override;

  [[nodiscard]]
  auto get_image_size() const -> Int2 override;

  [[nodiscard]]
  auto get_image_path() const -> const std::filesystem::path& override;

  [[nodiscard]]
  auto get_meta() const -> const IMetaView& override;

  [[nodiscard]]
  auto get_filename() const -> std::string override;

 private:
  const MapDocument* mDocument;
  EntityID mTilesetId;
  MetaViewImpl mMeta;
};

}  // namespace tactile::core
