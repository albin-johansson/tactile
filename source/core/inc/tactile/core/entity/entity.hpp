// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include <entt/entity/entity.hpp>

#include "tactile/base/prelude.hpp"

namespace tactile {

using EntityID = entt::entity;

inline constexpr EntityID kInvalidEntity = entt::null;

/**
 * Helper type intended to be inherited by tag component types.
 */
struct CTag
{
  char unused [[maybe_unused]];
};

/**
 * Returns a string that describes an entity identifier.
 *
 * \param entity An entity identifier.
 *
 * \return
 * A string describing an entity identifier.
 */
[[nodiscard]]
auto entity_to_string(EntityID entity) -> std::string;

}  // namespace tactile
