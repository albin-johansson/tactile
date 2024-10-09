// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class MapDocument;

/**
 * A command for removing tilesets from map documents.
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
   * \param document   The target map document, cannot be null.
   * \param tileset_id The target tileset identifier.
   */
  RemoveTilesetCommand(MapDocument* document, EntityID tileset_id);

  ~RemoveTilesetCommand() noexcept override;

  void undo() override;

  void redo() override;

 private:
  MapDocument* m_document;
  EntityID m_tileset_id;
  bool m_tileset_was_removed;
};

}  // namespace tactile::core
