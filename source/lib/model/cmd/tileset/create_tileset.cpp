// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "create_tileset.hpp"

#include <memory>   // make_shared
#include <utility>  // move

#include "core/tile/tileset_bundle.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

CreateTileset::CreateTileset(DocumentModel* model,
                             const UUID& map_id,
                             const UUID& tileset_id,
                             TilesetInfo info)
    : mModel {model},
      mMapId {map_id},
      mTilesetId {tileset_id},
      mTilesetInfo {std::move(info)}
{
  if (!mModel) {
    throw Exception {"Invalid null model!"};
  }
}

void CreateTileset::undo()
{
  if (mModel->is_open(mTilesetId)) {
    mModel->close_document(mTilesetId);
  }

  TACTILE_ASSERT(!mModel->is_open(mTilesetId));
  TACTILE_ASSERT(mModel->is_tileset(mTilesetId));

  auto document = mModel->get_map_document_ptr(mMapId);

  auto& tilesets = document->get_map().get_tileset_bundle();
  tilesets.detach_tileset(mTilesetId);

  document->get_contexts().remove_context(mTilesetId);
}

void CreateTileset::redo()
{
  // We only need to create and register the document once
  if (!mTileset) {
    mTileset = std::make_shared<TilesetDocument>(mTilesetInfo, mTilesetId);
    mTileset->set_name(mTilesetInfo.texture->get_path().stem().string());
    mModel->register_tileset(mTileset);
  }

  TACTILE_ASSERT(mModel->is_tileset(mTilesetId));
  TACTILE_ASSERT(mModel->is_map(mMapId));

  auto document = mModel->get_map_document_ptr(mMapId);
  auto& map = document->get_map();

  auto tileset = mTileset->get_tileset_ptr();
  map.get_tileset_bundle().attach_tileset(tileset, false);

  document->get_contexts().add_context(tileset);
}

auto CreateTileset::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_tileset;
}

}  // namespace tactile::cmd
