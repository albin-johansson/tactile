// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for removing layers from maps.
 */
class RemoveLayerCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The layer identifier must be valid.
   *
   * \param document The host document, cannot be null.
   * \param layer_id The target layer.
   */
  RemoveLayerCommand(MapDocument* document, EntityID layer_id);

  void undo() override;

  void redo() override;

  void dispose() override;

 private:
  MapDocument* m_document;
  EntityID m_layer_id;
  EntityID m_parent_layer_id;
  bool m_layer_was_removed;
};

}  // namespace tactile
