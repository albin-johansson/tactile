// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, ComponentIndex)

namespace tactile::cmd {

/// A command for renaming a component definition.
class RenameComponent final : public Command {
 public:
  RenameComponent(Shared<ComponentIndex> index, const UUID& component_id, String name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<ComponentIndex> mComponentIndex;
  UUID mComponentId {};
  String mNewName;
  Maybe<String> mOldName;
};

}  // namespace tactile::cmd
