// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class MapDocument;

/**
 * A command for moving layers downward in map layer hierarchies.
 */
class MoveLayerDownCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The layer identifier must be valid.
   *
   * \param document The host document, cannot be null.
   * \param layer_id The target layer identifier.
   */
  MoveLayerDownCommand(MapDocument* document, EntityID layer_id);

  void undo() override;

  void redo() override;

 private:
  MapDocument* m_document;
  EntityID m_layer_id;
};

}  // namespace tactile::core
