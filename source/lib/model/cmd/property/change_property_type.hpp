// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Context)

namespace tactile::cmd {

class ChangePropertyType final : public Command {
 public:
  ChangePropertyType(Shared<Context> context, String name, AttributeType type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Context> mContext;
  String mName;
  AttributeType mPropertyType;
  Maybe<Attribute> mPreviousValue;
};

}  // namespace tactile::cmd
