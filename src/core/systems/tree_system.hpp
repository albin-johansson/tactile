#pragma once

#include <entt.hpp>  // registry, entity

#include "common/ints.hpp"

namespace Tactile::Sys::Tree {

void MoveNodeUp(entt::registry& registry, entt::entity entity);

void MoveNodeDown(entt::registry& registry, entt::entity entity);

[[nodiscard]] auto CanMoveNodeUp(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto CanMoveNodeDown(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto GetSiblingAbove(const entt::registry& registry, entt::entity entity)
    -> entt::entity;

[[nodiscard]] auto GetSiblingBelow(const entt::registry& registry, entt::entity entity)
    -> entt::entity;

[[nodiscard]] auto GetSiblingCount(const entt::registry& registry, entt::entity entity)
    -> usize;

[[nodiscard]] auto GetChildrenCount(const entt::registry& registry, entt::entity entity)
    -> usize;

}  // namespace Tactile::Sys::Tree