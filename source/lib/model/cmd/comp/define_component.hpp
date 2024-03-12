// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component_definition.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, ComponentIndex)

namespace tactile::cmd {

/// A command for defining new components.
class DefineComponent final : public Command {
 public:
  DefineComponent(Shared<ComponentIndex> index, String name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<ComponentIndex> mIndex;
  String mName;
  Maybe<ComponentDefinition> mDef;
};

}  // namespace tactile::cmd
