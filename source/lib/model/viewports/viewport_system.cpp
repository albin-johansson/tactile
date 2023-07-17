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

#include "viewport_system.hpp"

#include "model/documents/document_system.hpp"
#include "model/viewports/viewport_components.hpp"
#include "model/viewports/viewport_ops.hpp"

namespace tactile::sys {

auto is_viewport_zoom_out_possible(const Registry& registry) -> bool
{
  const auto document_entity = get_active_document(registry);

  if (document_entity != kNullEntity) {
    const auto& viewport = registry.get<Viewport>(document_entity);
    return can_zoom_out_in(viewport);
  }

  return false;
}

}  // namespace tactile::sys
