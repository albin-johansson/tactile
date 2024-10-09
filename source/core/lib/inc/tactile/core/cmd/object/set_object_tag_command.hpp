// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/document/document.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

/**
 * A command for changing the type tag of objects.
 */
class SetObjectTagCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The object identifier must be valid.
   *
   * \param document  The host document, cannot be null.
   * \param object_id The target object identifier.
   * \param tag       The new type tag.
   */
  SetObjectTagCommand(IDocument* document, EntityID object_id, std::string tag);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* cmd) -> bool override;

 private:
  IDocument* m_document;
  EntityID m_object_id;
  std::string m_new_tag;
  std::string m_old_tag;
};

}  // namespace tactile::core
