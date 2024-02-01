// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class IDocument;

/**
 * A command for updating the values of meta context properties.
 */
class TACTILE_CORE_API UpdatePropertyValueCommand final : public ICommand {
 public:
  /**
   * Creates a new command.
   *
   * \param document       The host document.
   * \param context_uuid   The target context UUID.
   * \param property_name  The target property name.
   * \param property_value The new property value.
   */
  UpdatePropertyValueCommand(IDocument* document,
                             const UUID& context_uuid,
                             String property_name,
                             Attribute property_value);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* other) -> bool override;

 private:
  IDocument* mDocument;
  UUID mContextUuid;
  String mPropertyName;
  Attribute mNewPropertyValue;
  Maybe<Attribute> mOldPropertyValue {};
};

}  // namespace tactile::core
