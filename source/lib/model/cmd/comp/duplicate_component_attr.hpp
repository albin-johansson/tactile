// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Document)

namespace tactile::cmd {

/// A command for duplicating an attribute in a component definition.
class DuplicateComponentAttr final : public Command {
 public:
  DuplicateComponentAttr(Document* document, const UUID& component_id, String attribute);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Document* mDocument {};
  UUID mComponentId {};
  String mAttributeName;
  Maybe<String> mDuplicatedName;
};

}  // namespace tactile::cmd
