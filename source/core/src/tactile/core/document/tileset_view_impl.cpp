// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/tileset_view_impl.hpp"

#include <algorithm>  // count_if

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/document/tile_view_impl.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/io/texture.hpp"
#include "tactile/core/tile/tile.hpp"
#include "tactile/core/tile/tileset.hpp"

namespace tactile {

TilesetViewImpl::TilesetViewImpl(const MapDocument* document,
                                 const EntityID tileset_id)
  : mDocument {require_not_null(document, "null document")},
    mTilesetId {tileset_id}
{}

void TilesetViewImpl::accept(IDocumentVisitor& visitor) const
{
  visitor.visit(*this);

  const auto& registry = mDocument->get_registry();
  const auto& tileset = registry.get<CTileset>(mTilesetId);

  for (const auto tile_id : tileset.tiles) {
    if (!is_tile_plain(registry, tile_id)) {
      const TileViewImpl tile_view {mDocument, this, tile_id};
      tile_view.accept(visitor);
    }
  }
}

auto TilesetViewImpl::get_first_tile_id() const -> TileID
{
  const auto& registry = mDocument->get_registry();
  const auto& instance = registry.get<CTilesetInstance>(mTilesetId);

  return instance.tile_range.first_id;
}

auto TilesetViewImpl::tile_count() const -> usize
{
  const auto& registry = mDocument->get_registry();
  const auto& tileset = registry.get<CTileset>(mTilesetId);

  return tileset.tiles.size();
}

auto TilesetViewImpl::tile_definition_count() const -> usize
{
  const auto& registry = mDocument->get_registry();
  const auto& tileset = registry.get<CTileset>(mTilesetId);

  return saturate_cast<usize>(
      std::ranges::count_if(tileset.tiles, [&registry](const EntityID tile_id) {
        return !is_tile_plain(registry, tile_id);
      }));
}

auto TilesetViewImpl::column_count() const -> usize
{
  const auto& registry = mDocument->get_registry();

  const auto& tileset = registry.get<CTileset>(mTilesetId);
  const auto& texture = registry.get<CTexture>(mTilesetId);

  return saturate_cast<usize>(texture.size.x() / tileset.tile_size.x());
}

auto TilesetViewImpl::get_tile_size() const -> Int2
{
  const auto& registry = mDocument->get_registry();
  const auto& tileset = registry.get<CTileset>(mTilesetId);

  return tileset.tile_size;
}

auto TilesetViewImpl::get_image_size() const -> Int2
{
  const auto& registry = mDocument->get_registry();
  const auto& texture = registry.get<CTexture>(mTilesetId);

  return texture.size;
}

auto TilesetViewImpl::get_image_path() const -> const Path&
{
  const auto& registry = mDocument->get_registry();
  const auto& texture = registry.get<CTexture>(mTilesetId);

  return texture.path;
}

}  // namespace tactile
