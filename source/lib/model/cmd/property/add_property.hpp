// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/attribute.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Context)

namespace tactile::cmd {

class AddProperty final : public Command {
 public:
  AddProperty(Shared<Context> context, String name, AttributeType type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Context> mContext;
  String mName;
  AttributeType mType;
};

}  // namespace tactile::cmd
