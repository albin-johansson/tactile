// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class IDocument;

/**
 * A command for updating property values.
 */
class UpdatePropertyCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The meta context identifier must be valid.
   *
   * \param document       The host document, cannot be null.
   * \param context_id     The target meta context identifier.
   * \param property_name  The name of the target property.
   * \param property_value The new property value.
   */
  UpdatePropertyCommand(IDocument* document,
                        EntityID context_id,
                        std::string property_name,
                        Attribute property_value);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* cmd) -> bool override;

 private:
  IDocument* m_document;
  EntityID m_context_id;
  std::string m_property_name;
  Attribute m_new_property_value;
  Attribute m_old_property_value;
};

}  // namespace tactile
