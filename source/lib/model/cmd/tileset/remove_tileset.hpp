// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

/// Command for removing a tileset from the active map document.
class RemoveTileset final : public Command {
 public:
  RemoveTileset(DocumentModel* model, const UUID& tileset_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  DocumentModel* mModel {};
  UUID mTilesetId {};
  UUID mMapId {};
  Shared<TilesetDocument> mTilesetDocument;
  Maybe<TileID> mFirstTile;
};

}  // namespace tactile::cmd
