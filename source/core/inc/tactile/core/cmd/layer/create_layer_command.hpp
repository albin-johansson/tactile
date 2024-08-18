// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for creating new map layers.
 */
class CreateLayerCommand final : public ICommand
{
 public:
  TACTILE_DELETE_COPY(CreateLayerCommand);
  TACTILE_DEFAULT_MOVE(CreateLayerCommand);

  /**
   * Creates a command.
   *
   * \param document The parent map document.
   * \param type     The type of the new layer.
   */
  CreateLayerCommand(MapDocument* document, LayerType type);

  ~CreateLayerCommand() noexcept override;

  void undo() override;

  void redo() override;

 private:
  MapDocument* mDocument;
  LayerType mType;
  EntityID mParentLayerId;
  EntityID mLayerId;
  bool mWasLayerAdded;
};

}  // namespace tactile
