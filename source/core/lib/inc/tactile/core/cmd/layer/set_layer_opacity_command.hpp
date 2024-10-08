// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for changing the opacity of map layers.
 */
class SetLayerOpacityCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The layer identifier must be valid.
   *
   * \param document The host document, cannot be null.
   * \param layer_id The target layer identifier.
   * \param opacity  The new layer opacity.
   */
  SetLayerOpacityCommand(MapDocument* document, EntityID layer_id, float opacity);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* cmd) -> bool override;

 private:
  MapDocument* m_document;
  EntityID m_layer_id;
  float m_new_opacity;
  float m_old_opacity;
};

}  // namespace tactile
