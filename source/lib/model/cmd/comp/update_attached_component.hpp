// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Context)

namespace tactile::cmd {

/// A command for updating the attribute of a component attached to a context.
class UpdateAttachedComponent final : public Command {
 public:
  UpdateAttachedComponent(Shared<Context> context,
                          const UUID& component_id,
                          String attribute,
                          Attribute value);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Context> mContext;
  UUID mComponentId {};
  String mAttributeName;
  Attribute mUpdatedValue;
  Maybe<Attribute> mPreviousValue;
};

}  // namespace tactile::cmd
