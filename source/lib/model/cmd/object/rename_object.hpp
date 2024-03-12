// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/object.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile::cmd {

class RenameObject final : public Command {
 public:
  RenameObject(Shared<Object> object, String name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Object> mObject;
  String mNewName;
  Maybe<String> mOldName;
};

}  // namespace tactile::cmd
