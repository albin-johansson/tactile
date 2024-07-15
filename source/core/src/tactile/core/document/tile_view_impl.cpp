// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/tile_view_impl.hpp"

#include "tactile/base/document/document_visitor.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/base/document/document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/tile/animation.hpp"
#include "tactile/core/tile/tile.hpp"
#include "tactile/core/tile/tileset.hpp"

namespace tactile {

TileViewImpl::TileViewImpl(const IDocument* document,
                           const ITilesetView* tileset_view,
                           const EntityID tile_id)
  : mDocument {require_not_null(document, "null document")},
    mTilesetView {require_not_null(tileset_view, "null tileset view")},
    mTileId {tile_id},
    mMeta {document, tile_id}
{}

void TileViewImpl::accept(IDocumentVisitor& visitor) const
{
  visitor.visit(*this);

  const auto& registry = mDocument->get_registry();
  const auto& tile = registry.get<CTile>(mTileId);

  for (const auto object_id : tile.objects) {
    const ObjectViewImpl object_view {mDocument, this, object_id};
    object_view.accept(visitor);
  }
}

auto TileViewImpl::get_parent_tileset() const -> const ITilesetView&
{
  return *mTilesetView;
}

auto TileViewImpl::get_index() const -> TileIndex
{
  const auto& registry = mDocument->get_registry();
  const auto& tile = registry.get<CTile>(mTileId);

  return tile.index;
}

auto TileViewImpl::object_count() const -> usize
{
  const auto& registry = mDocument->get_registry();
  const auto& tile = registry.get<CTile>(mTileId);

  return tile.objects.size();
}

auto TileViewImpl::animation_frame_count() const -> usize
{
  const auto& registry = mDocument->get_registry();

  if (const auto* animation = registry.find<CAnimation>(mTileId)) {
    return animation->frames.size();
  }

  return 0;
}

auto TileViewImpl::get_animation_frame(const usize index) const
    -> Pair<TileIndex, Milliseconds>
{
  const auto& registry = mDocument->get_registry();

  const auto& animation = registry.get<CAnimation>(mTileId);
  const auto& frame = animation.frames.at(index);

  return {frame.tile_index, frame.duration};
}

auto TileViewImpl::get_meta() const -> const IMetaView&
{
  return mMeta;
}

}  // namespace tactile
