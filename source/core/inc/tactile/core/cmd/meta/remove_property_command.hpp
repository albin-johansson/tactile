// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/meta/attribute.hpp"

namespace tactile {

class IDocument;

/**
 * A command for removing a property from a meta context.
 */
class RemovePropertyCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \param document       The parent document.
   * \param context_entity The target meta context.
   * \param name           The name of the property to remove.
   */
  RemovePropertyCommand(IDocument* document,
                        EntityID context_entity,
                        String name);

  void undo() override;

  void redo() override;

 private:
  IDocument* mDocument;
  EntityID mContextEntity;
  String mName;
  Optional<Attribute> mValue;
};

}  // namespace tactile
