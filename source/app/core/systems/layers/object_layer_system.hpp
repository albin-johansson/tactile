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

#include <utility>  // pair

#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/components/layers.hpp"

namespace tactile::sys {

/**
 * \ingroup layer-system
 * \defgroup object-layer-system Object Layer System
 */

/// \addtogroup object-layer-system
/// \{

/// \name Object functions
/// \{

[[nodiscard]] auto get_object_layer(entt::registry& registry, LayerID id)
    -> std::pair<entt::entity, comp::ObjectLayer&>;

[[nodiscard]] auto get_object_layer(const entt::registry& registry, LayerID id)
    -> std::pair<entt::entity, const comp::ObjectLayer&>;

[[nodiscard]] auto has_object(const entt::registry& registry,
                              const comp::ObjectLayer& layer,
                              ObjectID id) -> bool;

[[nodiscard]] auto find_object(const entt::registry& registry,
                               const comp::ObjectLayer& layer,
                               ObjectID id) -> entt::entity;

/**
 * \brief Finds an object in the specified layer that contains the supplied coordinates.
 *
 * \details This function is designed to be used to find objects at the location that the
 * user has clicked in the map. As a result, this function takes care of the relationship
 * between viewport coordinates and logical coordinates. It also handles aspects such as
 * points having no width/height, so that points can be detected using this approach as
 * well.
 *
 * \param registry the current map registry.
 * \param layer the source object layer.
 * \param x the viewport relative x-coordinate.
 * \param y the viewport relative y-coordinate.
 *
 * \return an object entity that contains the specified coordinates;
 *         a null entity is returned if there is none.
 */
[[nodiscard]] auto find_object(const entt::registry& registry,
                               const comp::ObjectLayer& layer,
                               float x,
                               float y) -> entt::entity;

/// \} End of object functions

/// \} End of group object-layer-system

}  // namespace tactile::sys
