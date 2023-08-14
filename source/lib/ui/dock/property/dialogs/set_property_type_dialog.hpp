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

#include "core/ecs/entity.hpp"
#include "tactile/core/containers/string.hpp"
#include "core/enums/attribute_type.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "model/model_view.hpp"

namespace tactile {

struct SetPropertyTypeDialogState final {
  Entity context {kNullEntity};
  AttributeType current_type {AttributeType::String};
  Maybe<String> property_name;
  Maybe<AttributeType> old_type;
  bool should_open {};
};

void push_set_property_type_dialog(ModelView& model, SetPropertyTypeDialogState& state);

}  // namespace tactile
