// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class MapDocument;

/**
 * A command for creating objects.
 */
class CreateObjectCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The object layer identifier must be valid.
   *
   * \param document The host document, cannot be null.
   * \param layer_id The parent object layer identifier.
   * \param type     The object type.
   * \param position The object position.
   * \param size     The object size.
   */
  CreateObjectCommand(MapDocument* document,
                      EntityID layer_id,
                      ObjectType type,
                      const Float2& position,
                      const Float2& size);

  void undo() override;

  void redo() override;

  void dispose() override;

 private:
  MapDocument* m_document;
  EntityID m_layer_id;
  ObjectType m_type;
  Float2 m_position;
  Float2 m_size;
  EntityID m_object_id;
  bool m_object_was_added;
};

}  // namespace tactile
