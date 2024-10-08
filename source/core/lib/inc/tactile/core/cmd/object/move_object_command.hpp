// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class IDocument;

/**
 * A command for changing the position of objects.
 */
class MoveObjectCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The object identifier must be valid.
   *
   * \param document  The host document, cannot be null.
   * \param object_id The target object identifier.
   * \param position  The new position of the object.
   */
  MoveObjectCommand(IDocument* document, EntityID object_id, const Float2& position);

  void undo() override;

  void redo() override;

 private:
  IDocument* m_document;
  EntityID m_object_id;
  Float2 m_new_position;
  Float2 m_old_position;
};

}  // namespace tactile
