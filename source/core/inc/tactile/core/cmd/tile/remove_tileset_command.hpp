// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for removing a tileset from a map document.
 */
class RemoveTilesetCommand final : public ICommand
{
 public:
  TACTILE_DELETE_COPY(RemoveTilesetCommand);
  TACTILE_DEFAULT_MOVE(RemoveTilesetCommand);

  /**
   * Creates a command.
   *
   * \pre The tileset identifier must be valid.
   *
   * \param document   The target map document.
   * \param tileset_id The entity identifier of the tileset to remove.
   */
  RemoveTilesetCommand(MapDocument* document, EntityID tileset_id);

  ~RemoveTilesetCommand() noexcept override;

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  EntityID mTilesetId;
  bool mTilesetWasRemoved;
};

}  // namespace tactile
