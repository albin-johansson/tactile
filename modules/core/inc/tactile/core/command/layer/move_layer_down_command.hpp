// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/misc/uuid.hpp"

namespace tactile::core {

class MapDocument;

/**
 * A command for moving layers in maps downwards.
 */
class TACTILE_CORE_API MoveLayerDownCommand final : public ICommand {
 public:
  /**
   * Creates a command.
   *
   * \param document   The host document, cannot be null.
   * \param layer_uuid The target layer UUID.
   */
  MoveLayerDownCommand(MapDocument* document, const UUID& layer_uuid);

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  UUID mLayerUuid;
};

}  // namespace tactile::core
