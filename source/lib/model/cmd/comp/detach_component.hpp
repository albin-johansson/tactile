// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Context)

namespace tactile::cmd {

/// A command for detaching a component from a context.
class DetachComponent final : public Command {
 public:
  DetachComponent(Shared<Context> context, const UUID& component_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Context> mContext;
  UUID mComponentId {};
  Maybe<Component> mComponent;
};

}  // namespace tactile::cmd
