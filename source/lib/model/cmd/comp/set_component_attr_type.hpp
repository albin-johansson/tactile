/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "common/macros.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/maybe.hpp"
#include "common/type/uuid.hpp"
#include "core/attribute.hpp"
#include "model/cmd/command.hpp"

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
