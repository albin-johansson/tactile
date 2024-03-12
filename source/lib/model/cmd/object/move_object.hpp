// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/object.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile::cmd {

/// Command for moving an object in a map.
class MoveObject final : public Command {
 public:
  MoveObject(Shared<Object> object, const Float2& previous, const Float2& updated);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Object> mObject;
  Float2 mPreviousPos {};
  Float2 mUpdatedPos {};
};

}  // namespace tactile::cmd
