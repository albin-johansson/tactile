// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/tile/tileset_types.hpp"

namespace tactile::core {

class MapDocument;

/**
 * A command for adding tilesets to map documents.
 */
class AddTilesetCommand final : public ICommand
{
 public:
  TACTILE_DELETE_COPY(AddTilesetCommand);
  TACTILE_DEFAULT_MOVE(AddTilesetCommand);

  /**
   * Creates a command.
   *
   * \param document The target map document, cannot be null.
   * \param spec     The tileset specification.
   */
  AddTilesetCommand(MapDocument* document, TilesetSpec spec);

  ~AddTilesetCommand() noexcept override;

  void undo() override;

  void redo() override;

 private:
  MapDocument* m_document;
  TilesetSpec m_spec;
  EntityID m_tileset_id;
  bool m_tileset_was_added;
};

}  // namespace tactile::core
