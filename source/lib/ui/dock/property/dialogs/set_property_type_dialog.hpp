/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/enum/attribute_type.hpp"
#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "common/type/string.hpp"
#include "model/registry.hpp"

namespace tactile::ui {

struct SetPropertyTypeDialogState final {
  Entity context {kNullEntity};
  AttributeType current_type {AttributeType::String};
  Maybe<String> property_name;
  Maybe<AttributeType> old_type;
  bool should_open {};
};

void push_set_property_type_dialog(const Registry& model,
                                   SetPropertyTypeDialogState& state,
                                   Dispatcher& dispatcher);

}  // namespace tactile::ui
