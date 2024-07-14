// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Document)

namespace tactile::cmd {

/// A command for removing an attribute from a component definition.
class RemoveComponentAttr final : public Command {
 public:
  RemoveComponentAttr(Document* document, const UUID& component_id, String attribute);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Document* mDocument {};
  UUID mComponentId {};
  String mAttributeName;
  Maybe<Attribute> mPreviousValue;
};

}  // namespace tactile::cmd
