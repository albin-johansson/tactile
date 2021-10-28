#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry, entity

namespace Tactile::Sys::LayerTree {

void SortNodes(entt::registry& registry);

void IncrementIndicesOfSiblingsBelow(entt::registry& registry, entt::entity entity);

void DecrementIndicesOfSiblingsBelow(entt::registry& registry, entt::entity entity);

void DestroyNode(entt::registry& registry, entt::entity entity);

void MoveNodeUp(entt::registry& registry, entt::entity entity);

void MoveNodeDown(entt::registry& registry, entt::entity entity);

[[nodiscard]] auto CanMoveNodeUp(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto CanMoveNodeDown(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto IsChildNode(const entt::registry& registry,
                               entt::entity parent,
                               entt::entity entity) -> bool;

[[nodiscard]] auto GetSiblingAbove(const entt::registry& registry, entt::entity entity)
    -> entt::entity;

[[nodiscard]] auto GetSiblingBelow(const entt::registry& registry, entt::entity entity)
    -> entt::entity;

[[nodiscard]] auto GetSiblingCount(const entt::registry& registry, entt::entity entity)
    -> usize;

[[nodiscard]] auto GetChildrenCount(const entt::registry& registry, entt::entity entity)
    -> usize;

[[nodiscard]] auto GetGlobalIndex(const entt::registry& registry, entt::entity entity)
    -> usize;

}  // namespace Tactile::Sys::LayerTree