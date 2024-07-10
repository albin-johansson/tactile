// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, ComponentIndex)

namespace tactile::cmd {

/// Command for changing the default value of an attribute in a component.
class UpdateComponent final : public Command {
 public:
  UpdateComponent(Shared<ComponentIndex> index,
                  const UUID& component_id,
                  String attribute,
                  Attribute value);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<ComponentIndex> mIndex;
  UUID mComponentId {};
  String mAttributeName;
  Attribute mUpdatedValue;
  Maybe<Attribute> mPreviousValue;
};

}  // namespace tactile::cmd
