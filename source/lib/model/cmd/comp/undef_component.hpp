// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "core/component/component.hpp"
#include "core/component/component_definition.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Document)

namespace tactile::cmd {

/// A command for removing a component definition.
class UndefComponent final : public Command {
 public:
  UndefComponent(Document* document, const UUID& component_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Document* mDocument {};
  UUID mComponentId {};
  Maybe<ComponentDefinition> mPreviousDef;
  HashMap<UUID, Component> mRemovedComponents;
};

}  // namespace tactile::cmd
