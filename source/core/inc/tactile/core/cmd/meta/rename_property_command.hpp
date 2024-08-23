// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class IDocument;

/**
 * A command for renaming properties attached to meta contexts.
 */
class RenamePropertyCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The meta context identifier must be valid.
   * \pre The new name must not be used by another property in the context.
   *
   * \param document   The host document, cannot be null.
   * \param context_id The target meta context identifier.
   * \param old_name   The name of the target property.
   * \param new_name   The new name of the property.
   */
  RenamePropertyCommand(IDocument* document,
                        EntityID context_id,
                        std::string old_name,
                        std::string new_name);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* cmd) -> bool override;

 private:
  IDocument* m_document;
  EntityID m_context_id;
  std::string m_old_name;
  std::string m_new_name;

  void _rename_property(std::string_view from, std::string to);
};

}  // namespace tactile
