// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for removing objects from object layers.
 */
class RemoveObjectCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The object layer identifier must be valid.
   * \pre The object identifier must be valid.
   *
   * \param document  The host document, cannot be null.
   * \param layer_id  The parent layer identifier.
   * \param object_id The target object identifier.
   */
  RemoveObjectCommand(MapDocument* document, EntityID layer_id, EntityID object_id);

  void undo() override;

  void redo() override;

  void dispose() override;

 private:
  MapDocument* m_document;
  EntityID m_layer_id;
  EntityID m_object_id;
  bool m_object_was_removed;
};

}  // namespace tactile
