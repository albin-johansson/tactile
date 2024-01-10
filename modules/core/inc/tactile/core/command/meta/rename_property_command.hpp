// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class IDocument;

/**
 * A command for renaming properties in meta contexts.
 */
class TACTILE_CORE_API RenamePropertyCommand final : public ICommand {
 public:
  /**
   * Creates a new command.
   *
   * \param document          The host document.
   * \param context_uuid      The target context UUID.
   * \param old_property_name The old name of the property.
   * \param new_property_name The new name of the property.
   */
  RenamePropertyCommand(IDocument* document,
                        const UUID& context_uuid,
                        String old_property_name,
                        String new_property_name);

  void undo() override;

  void redo() override;

 private:
  IDocument* mDocument;
  UUID mContextUuid;
  String mOldPropertyName;
  String mNewPropertyName;
};

}  // namespace tactile
