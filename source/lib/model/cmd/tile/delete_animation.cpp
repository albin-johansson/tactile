// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "delete_animation.hpp"

#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

DeleteAnimation::DeleteAnimation(TilesetDocument* document, const TileIndex tile_index)
    : mDocument {document},
      mTileIndex {tile_index}
{
  if (!mDocument) {
    throw Exception {"Invalid null tileset document"};
  }
}

void DeleteAnimation::undo()
{
  auto& tileset = mDocument->get_tileset();

  auto& tile = tileset[mTileIndex];
  tile.set_animation(mRemovedAnimation.value());

  mRemovedAnimation.reset();
}

void DeleteAnimation::redo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];

  mRemovedAnimation = tile.get_animation();
  tile.clear_animation();
}

auto DeleteAnimation::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.delete_animation;
}

}  // namespace tactile::cmd
