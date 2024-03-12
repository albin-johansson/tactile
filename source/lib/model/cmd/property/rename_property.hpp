// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Context)

namespace tactile::cmd {

class RenameProperty final : public Command {
 public:
  RenameProperty(Shared<Context> context, String old_name, String new_name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Context> mContext;
  String mOldName;
  String mNewName;
};

}  // namespace tactile::cmd
