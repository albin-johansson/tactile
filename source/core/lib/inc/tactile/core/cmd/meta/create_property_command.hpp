// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <optional>
#include <string>

#include "tactile/base/document/document.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/cmd/command.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile::core {

/**
 * A command for creating properties and adding them to meta contexts.
 */
class CreatePropertyCommand final : public ICommand
{
 public:
  /**
   * Creates a command.
   *
   * \pre The meta context identifier must be valid.
   *
   * \param document   The host document, cannot be null.
   * \param context_id The target meta context identifier.
   * \param name       The name of the property.
   * \param value      The value of the property.
   */
  CreatePropertyCommand(IDocument* document,
                        EntityID context_id,
                        std::string name,
                        Attribute value);

  void undo() override;

  void redo() override;

 private:
  IDocument* m_document;
  EntityID m_context_id;
  std::string m_name;
  Attribute m_value;
};

}  // namespace tactile::core
