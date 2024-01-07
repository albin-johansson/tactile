// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class IDocument;

/**
 * Command for renaming a meta context.
 */
class TACTILE_CORE_API RenameMetaContextCommand final : public ICommand {
 public:
  /**
   * Creates a new command.
   *
   * \param document     The associated document, must outlive the command.
   * \param context_uuid The UUID associated with the target meta context.
   * \param new_name     The new name of the context.
   */
  RenameMetaContextCommand(IDocument* document,
                           const UUID& context_uuid,
                           String new_name);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* other) -> bool override;

 private:
  IDocument* mDocument;
  UUID mContextUuid;
  String mNewName;
  Maybe<String> mOldName {};
};

}  // namespace tactile
