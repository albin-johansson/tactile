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

#include <centurion/fwd.hpp>
#include <entt/fwd.hpp>

#include "editor/fwd.hpp"

namespace tactile {

void render_object(GraphicsCtx& graphics,
                   const entt::registry& registry,
                   entt::entity objectEntity,
                   const cen::color& color);

/**
 * \brief Renders an object layer.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the associated registry.
 * \param layerEntity the object layer entity.
 * \param parentOpacity the opacity of the parent layer.
 */
void render_object_layer(GraphicsCtx& graphics,
                         const entt::registry& registry,
                         entt::entity layerEntity,
                         float parentOpacity);

}  // namespace tactile
