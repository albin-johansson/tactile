// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/meta/attribute_type.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class IDocument;

/**
 * A command for updating the types of meta context properties.
 */
class TACTILE_CORE_API UpdatePropertyTypeCommand final : public ICommand {
 public:
  /**
   * Creates a new command.
   *
   * \param document      The host document.
   * \param context_uuid  The target context UUID.
   * \param property_name The target property name.
   * \param property_type The new property type.
   */
  UpdatePropertyTypeCommand(IDocument* document,
                            const UUID& context_uuid,
                            String property_name,
                            AttributeType property_type);

  void undo() override;

  void redo() override;

 private:
  IDocument* mDocument;
  UUID mContextUuid;
  String mPropertyName;
  AttributeType mNewPropertyType;
  Maybe<Attribute> mOldPropertyValue;
};

}  // namespace tactile
