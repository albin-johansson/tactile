// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "core/attribute.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Document)

namespace tactile::cmd {

/// A command for changing the type of an attribute in a component definition.
class SetComponentAttrType final : public Command {
 public:
  SetComponentAttrType(Document* document,
                       const UUID& component_id,
                       String attribute,
                       AttributeType type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Document* mDocument {};
  UUID mComponentId {};
  String mAttributeName;
  AttributeType mNewType;
  Maybe<Attribute> mSnapshot;
  HashMap<UUID, Attribute> mPrevAttributes;
};

}  // namespace tactile::cmd
