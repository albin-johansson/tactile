// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/meta/attribute.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

class IDocument;

/**
 * A command for creating properties and adding them to meta contexts.
 */
class CreatePropertyCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \param document       The parent document.
   * \param context_entity The target meta context entity.
   * \param name           The name of the property.
   * \param value          The value of the property.
   */
  CreatePropertyCommand(IDocument* document,
                        EntityID context_entity,
                        String name,
                        Attribute value);

  void undo() override;

  void redo() override;

 private:
  IDocument* mDocument;
  EntityID mContextEntity;
  String mName;
  Attribute mValue;
};

}  // namespace tactile
