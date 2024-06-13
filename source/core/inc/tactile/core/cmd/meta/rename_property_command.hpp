// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class IDocument;

/**
 * A command for renaming a property attached to a meta context.
 */
class RenamePropertyCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The new name must not be used by another property in the context.
   *
   * \param document       The parent document.
   * \param context_entity The target meta context.
   * \param old_name       The name of the target property.
   * \param new_name       The new name of the property.
   */
  RenamePropertyCommand(IDocument* document,
                        EntityID context_entity,
                        String old_name,
                        String new_name);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* cmd) -> bool override;

 private:
  IDocument* mDocument;
  EntityID mContextEntity;
  String mOldName;
  String mNewName;

  void _rename_property(StringView from, String to);
};

}  // namespace tactile
