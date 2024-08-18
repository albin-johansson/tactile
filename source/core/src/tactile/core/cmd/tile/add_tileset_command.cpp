// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/tile/add_tileset_command.hpp"

#include <exception>  // exception
#include <utility>    // move

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/tile/tileset.hpp"

namespace tactile {

AddTilesetCommand::AddTilesetCommand(MapDocument* document, TilesetSpec spec)
  : mDocument {require_not_null(document, "null document")},
    mSpec {std::move(spec)},
    mTilesetId {kInvalidEntity},
    mTilesetWasAdded {false}
{}

AddTilesetCommand::~AddTilesetCommand() noexcept
{
  try {
    if (!mTilesetWasAdded && mTilesetId != kInvalidEntity) {
      auto& registry = mDocument->get_registry();
      destroy_tileset(registry, mTilesetId);
    }
  }
  catch (const std::exception& error) {
    TACTILE_LOG_ERROR("Unexpected exception in AddTilesetCommand destructor: {}",
                      error.what());
  }
}

void AddTilesetCommand::undo()
{
  auto& registry = mDocument->get_registry();

  const auto& document_info = registry.get<CDocumentInfo>();
  const auto map_id = document_info.root;

  remove_tileset_from_map(registry, map_id, mTilesetId);

  mTilesetWasAdded = false;
}

void AddTilesetCommand::redo()
{
  auto& registry = mDocument->get_registry();

  const auto& document_info = registry.get<CDocumentInfo>();
  const auto map_id = document_info.root;

  if (mTilesetId == kInvalidEntity) {
    mTilesetId = add_tileset_to_map(registry, map_id, mSpec).value();
  }
  else {
    auto& map = registry.get<CMap>(map_id);
    map.attached_tilesets.push_back(mTilesetId);
    map.active_tileset = mTilesetId;
  }

  mTilesetWasAdded = true;
}

}  // namespace tactile
