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

#include "common/attribute.hpp"
#include "common/type/ecs.hpp"
#include "core/containers/string.hpp"
#include "core/containers/string_map.hpp"
#include "core/functional/maybe.hpp"
#include "model/components/component_components.hpp"
#include "model/contexts/context_components.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

/// Creates a component entity and attaches it to the specified context.
auto attach_component(Registry& registry, Context& context, Entity definition_entity)
    -> Entity;

/// Detaches a component from a context (this destroys the attached component entity).
auto detach_component(Registry& registry, Context& context, Entity definition_entity)
    -> Maybe<StringMap<Attribute>>;

[[nodiscard]] auto has_component(const Registry& registry,
                                 const Context& context,
                                 StringView component_name) -> bool;

}  // namespace tactile::sys
