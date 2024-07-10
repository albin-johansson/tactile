// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class IDocument;

/**
 * A command for updating the value of a property.
 */
class UpdatePropertyCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \param document       The parent document.
   * \param context_entity The target meta context.
   * \param property_name  The name of the target property.
   * \param property_value The new property value.
   */
  UpdatePropertyCommand(IDocument* document,
                        EntityID context_entity,
                        String property_name,
                        Attribute property_value);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* cmd) -> bool override;

 private:
  IDocument* mDocument;
  EntityID mContextEntity;
  String mPropertyName;
  Attribute mNewPropertyValue;
  Optional<Attribute> mOldPropertyValue;
};

}  // namespace tactile
