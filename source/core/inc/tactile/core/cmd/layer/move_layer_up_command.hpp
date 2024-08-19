// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for moving a layer upwards in a map layer hierarchy.
 */
class MoveLayerUpCommand final : public ICommand
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
  MoveLayerUpCommand(MapDocument* document, EntityID layer_id);

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  EntityID mLayerId;
};

}  // namespace tactile
