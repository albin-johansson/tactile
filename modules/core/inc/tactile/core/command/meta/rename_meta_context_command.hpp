// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/command/command.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class IMetaContext;

/**
 * \brief Command for renaming a meta context.
 */
class TACTILE_CORE_API RenameMetaContextCommand final : public ICommand {
 public:
  /**
   * \brief Creates a command.
   *
   * \param context  the target meta context.
   * \param new_name the new name of the meta context.
   */
  RenameMetaContextCommand(IMetaContext* context, String new_name);

  void undo() override;

  void redo() override;

  [[nodiscard]]
  auto merge_with(const ICommand* other) -> bool override;

 private:
  IMetaContext* mContext;
  IDocument* mDocument;
  String mNewName;
  Maybe<String> mOldName;
};

}  // namespace tactile
