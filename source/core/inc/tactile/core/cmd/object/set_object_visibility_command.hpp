// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class IDocument;

/**
 * A command for changing the visibility of objects.
 */
class SetObjectVisibilityCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The object identifier must be valid.
   *
   * \param document   The host document, cannot be null.
   * \param object_id  The target object identifier.
   * \param visibility The new visibility of the object.
   */
  SetObjectVisibilityCommand(IDocument* document, EntityID object_id, bool visibility);

  void undo() override;

  void redo() override;

 private:
  IDocument* m_document;
  EntityID m_object_id;
  bool m_new_visibility;
  bool m_old_visibility;
};

}  // namespace tactile
