// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/object.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile::cmd {

class SetObjectTag final : public Command {
 public:
  SetObjectTag(Shared<Object> object, String tag);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Object> mObject;
  String mNewTag;
  Maybe<String> mOldTag;
};

}  // namespace tactile::cmd
