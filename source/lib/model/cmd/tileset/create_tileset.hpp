// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/tile/tileset_info.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

/// Command for creating a tileset and attaching it to a map document.
class CreateTileset final : public Command {
 public:
  CreateTileset(DocumentModel* model,
                const UUID& map_id,
                const UUID& tileset_id,
                TilesetInfo info);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  DocumentModel* mModel {};
  UUID mMapId {};
  UUID mTilesetId {};
  TilesetInfo mTilesetInfo;
  Shared<TilesetDocument> mTileset;  /// The created tileset
};

}  // namespace tactile::cmd
