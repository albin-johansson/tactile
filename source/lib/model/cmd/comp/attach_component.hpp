// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Context)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, ComponentIndex)

namespace tactile::cmd {

/// Command for attaching a component to a context.
class AttachComponent final : public Command {
 public:
  AttachComponent(Shared<ComponentIndex> index,
                  Shared<Context> context,
                  const UUID& component_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<ComponentIndex> mIndex;
  Shared<Context> mContext;
  UUID mComponentId {};
};

}  // namespace tactile::cmd
