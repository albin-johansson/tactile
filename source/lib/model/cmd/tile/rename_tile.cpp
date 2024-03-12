// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_tile.hpp"

#include <utility>  // move

#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RenameTile::RenameTile(TilesetDocument* document, const TileIndex tile_index, String name)
    : mDocument {document},
      mTileIndex {tile_index},
      mNewName {std::move(name)}
{
  if (!mDocument) {
    throw Exception {"Invalid null tileset document"};
  }
}

void RenameTile::undo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];
  auto& tile_ctx = tile.get_ctx();

  tile_ctx.set_name(mOldName.value());
  mOldName.reset();
}

void RenameTile::redo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];
  auto& tile_ctx = tile.get_ctx();

  mOldName = tile_ctx.name();
  tile_ctx.set_name(mNewName);
}

auto RenameTile::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_tile;
}

}  // namespace tactile::cmd
