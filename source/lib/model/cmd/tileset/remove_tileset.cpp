// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "remove_tileset.hpp"

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

RemoveTileset::RemoveTileset(DocumentModel* model, const UUID& tileset_id)
    : mModel {model},
      mTilesetId {tileset_id}
{
  if (!mModel) {
    throw Exception {"Invalid null model!"};
  }

  TACTILE_ASSERT(mModel->get_active_document_id().has_value());
  mMapId = mModel->get_active_document_id().value();
}

void RemoveTileset::undo()
{
  TACTILE_ASSERT(mTilesetDocument != nullptr);
  TACTILE_ASSERT(mModel->get_active_document_id() == mMapId);

  auto map_document = mModel->get_map_document_ptr(mMapId);
  auto& map = map_document->get_map();

  auto tileset = mTilesetDocument->get_tileset_ptr();
  const auto tileset_id = tileset->get_uuid();

  auto& tilesets = map.get_tileset_bundle();
  tilesets.attach_tileset(tileset, mFirstTile.value(), false);  // TODO
  tilesets.select_tileset(tileset_id);

  map_document->get_contexts().add_context(std::move(tileset));
}

void RemoveTileset::redo()
{
  TACTILE_ASSERT(mModel->get_active_document_id() == mMapId);
  mTilesetDocument = mModel->get_tileset_document_ptr(mTilesetId);

  auto map_document = mModel->get_map_document_ptr(mMapId);
  auto& map = map_document->get_map();

  auto& tilesets = map.get_tileset_bundle();
  const auto& tileset_ref = tilesets.get_tileset_ref(mTilesetId);
  mFirstTile = tileset_ref.get_first_tile();

  if (tileset_ref.is_embedded()) {
    mModel->close_document(mTilesetId);
  }

  tilesets.detach_tileset(mTilesetId);
  map_document->get_contexts().remove_context(mTilesetId);
}

auto RemoveTileset::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_tileset;
}

}  // namespace tactile::cmd
