// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for duplicating a layer in a map.
 */
class DuplicateLayerCommand final : public ICommand
{
 public:
  TACTILE_DELETE_COPY(DuplicateLayerCommand);
  TACTILE_DEFAULT_MOVE(DuplicateLayerCommand);

  /**
   * Creates a command.
   *
   * \pre The layer identifier must be valid.
   *
   * \param document The host document, cannot be nul.
   * \param layer_id The target layer identifier.
   */
  DuplicateLayerCommand(MapDocument* document, EntityID layer_id);

  ~DuplicateLayerCommand() noexcept override;

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  EntityID mLayerId;
  EntityID mDuplicateLayerId;
  bool mLayerWasAdded;
};

}  // namespace tactile
