// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class MapDocument;

/**
 * A command for creating new map layers.
 */
class CreateLayerCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \param document The parent map document, cannot be null.
   * \param type     The type of the new layer.
   */
  CreateLayerCommand(MapDocument* document, LayerType type);

  void undo() override;

  void redo() override;

  void dispose() override;

 private:
  MapDocument* m_document;
  LayerType m_type;
  EntityID m_parent_layer_id;
  EntityID m_layer_id;
  bool m_layer_was_added;
};

}  // namespace tactile::core
