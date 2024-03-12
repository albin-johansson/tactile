// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/object.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile::cmd {

class SetObjectVisible final : public Command {
 public:
  SetObjectVisible(Shared<Object> object, bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Object> mObject;
  bool mNewVisibility;
  Maybe<bool> mOldVisibility;
};

}  // namespace tactile::cmd
