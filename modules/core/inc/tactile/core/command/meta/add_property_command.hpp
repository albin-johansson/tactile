// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/meta/attribute_type.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class IDocument;

/**
 * Command for adding properties to meta contexts.
 */
class TACTILE_CORE_API AddPropertyCommand final : public ICommand {
 public:
  /**
   * Creates a command.
   *
   * \param document     The host document, cannot be null.
   * \param context_uuid The UUID of the target context.
   * \param name         The name of the new property.
   * \param type         The type of the new property.
   */
  AddPropertyCommand(IDocument* document,
                     const UUID& context_uuid,
                     String name,
                     AttributeType type);

  void undo() override;

  void redo() override;

 private:
  IDocument* mDocument;
  UUID mContextUuid;
  String mPropertyName;
  AttributeType mPropertyType;
};

}  // namespace tactile::core
