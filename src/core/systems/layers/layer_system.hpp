#pragma once

#include <string>  // string

#include <entt.hpp>  // registry, entity, null

#include <tactile-base/layer_type.hpp>
#include "core/map.hpp"
#include "layer_snapshot.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile::Sys {

/// \name Layer system
/// \{

/**
 * \brief Creates a basic layer entity.
 *
 * \pre `parent` is either associated with a group layer or null.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `LayerTreeNode`
 * - `Parent`
 * - `PropertyContext`
 *
 * \param registry the associated registry.
 * \param id the unique identifier associated with the layer.
 * \param type the specific layer type.
 * \param name the name of the layer.
 * \param parent the parent layer entity, can safely be a null entity.
 *
 * \return the created layer entity.
 *
 * \since 0.2.0
 */
auto AddBasicLayer(entt::registry& registry,
                   LayerID id,
                   LayerType type,
                   std::string name,
                   entt::entity parent = entt::null) -> entt::entity;

/**
 * \brief Creates a tile layer entity.
 *
 * \details The created layer will be added as a child layer if the active layer at
 * the time of invocation is a group layer.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `TileLayer`
 * - `Parent`
 * - `PropertyContext`
 *
 * \param registry the associated registry.
 *
 * \return the created tile layer entity.
 *
 * \since 0.2.0
 */
auto AddTileLayer(entt::registry& registry) -> entt::entity;

/**
 * \brief Creates an object layer entity.
 *
 * \details The created layer will be added as a child layer if the active layer at
 * the time of invocation is a group layer.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `ObjectLayer`
 * - `Parent`
 * - `PropertyContext`
 *
 * \param registry the associated registry.
 *
 * \return the created object layer entity.
 *
 * \since 0.2.0
 */
auto AddObjectLayer(entt::registry& registry) -> entt::entity;

/**
 * \brief Creates a group layer entity.
 *
 * \details The created layer will be added as a child layer if the active layer at
 * the time of invocation is a group layer.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `GroupLayer`
 * - `Parent`
 * - `PropertyContext`
 *
 * \param registry the associated registry.
 *
 * \return the created group layer entity.
 *
 * \since 0.2.0
 */
auto AddGroupLayer(entt::registry& registry) -> entt::entity;

auto RestoreLayer(entt::registry& registry, LayerSnapshot snapshot) -> entt::entity;

/**
 * \brief Sorts all layers according to their current indices.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void SortLayers(entt::registry& registry);

/**
 * \brief Removes the layer associated with the specified ID.
 *
 * \pre `entity` must be associated with an existing layer.
 *
 * \details The active layer is is reset if the specified layer is active at the time
 * of invocation. This function will also repair the layer indices.
 *
 * \param registry the associated registry.
 * \param entity the layer entity that will be removed.
 *
 * \since 0.2.0
 */
void RemoveLayer(entt::registry& registry, entt::entity entity);

/**
 * \brief Selects the layer associated with the specified ID.
 *
 * \pre `entity` must be associated with an existing layer.
 *
 * \param registry the associated registry.
 * \param entity the entity associated with the layer that will be selected.
 *
 * \since 0.2.0
 */
void SelectLayer(entt::registry& registry, entt::entity entity);

[[nodiscard]] auto CopyLayer(const entt::registry& registry, entt::entity source)
    -> LayerSnapshot;

auto DuplicateLayer(entt::registry& registry, entt::entity source) -> entt::entity;

auto DuplicateLayer(entt::registry& registry,
                    entt::entity source,
                    entt::entity parent,
                    bool recursive) -> entt::entity;

/**
 * \brief Moves the specified layer "up", i.e. earlier in the rendering order.
 *
 * \pre `entity` must be associated with an existing layer.
 * \pre It must be possible to move the specified layer upwards.
 *
 * \details This function will repair the layer indices after moving the layer.
 *
 * \param registry the associated registry.
 * \param entity the entity associated with the layer that will be moved up.
 *
 * \see `CanMoveLayerUp()`
 *
 * \since 0.2.0
 */
void MoveLayerUp(entt::registry& registry, entt::entity entity);

/**
 * \brief Moves the specified layer "down", i.e. later in the rendering order.
 *
 * \pre `entity` must be associated with an existing layer.
 * \pre It must be possible to move the specified layer down.
 *
 * \details This function will repair the layer indices after moving the layer.
 *
 * \param registry the associated registry.
 * \param entity the entity associated with the layer that will be moved downwards.
 *
 * \see `CanMoveLayerDown()`
 *
 * \since 0.2.0
 */
void MoveLayerDown(entt::registry& registry, entt::entity entity);

/**
 * \brief Sets the opacity of the specified layer.
 *
 * \pre `entity` must be associated with an existing layer.
 *
 * \param registry the associated registry.
 * \param entity the entity associated with the layer that will be modified.
 * \param opacity the new opacity of the layer, in the range [0, 1].
 *
 * \since 0.2.0
 */
void SetLayerOpacity(entt::registry& registry, entt::entity entity, float opacity);

/**
 * \brief Sets the visibility of the specified layer.
 *
 * \pre `entity` must be associated with an existing layer.
 *
 * \param registry the associated registry.
 * \param entity the entity associated with the layer that will be modified.
 * \param visible the visibility of the layer.
 *
 * \since 0.2.0
 */
void SetLayerVisible(entt::registry& registry, entt::entity entity, bool visible);

/**
 * \brief Attempts to find the layer associated with the specified ID.
 *
 * \param registry the associated registry.
 * \param id the ID associated with the desired layer.
 *
 * \return the associated layer entity; a null entity is returned if there is none.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto FindLayer(const entt::registry& registry, LayerID id) -> entt::entity;

[[nodiscard]] auto GetActiveLayer(const entt::registry& registry) -> entt::entity;

[[nodiscard]] auto GetLayerIndex(const entt::registry& registry, entt::entity) -> usize;

[[nodiscard]] auto GetLayerOpacity(const entt::registry& registry, entt::entity entity)
    -> float;

[[nodiscard]] auto GetLayerId(const entt::registry& registry, entt::entity entity)
    -> LayerID;

[[nodiscard]] auto IsLayerVisible(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto CanMoveLayerUp(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto CanMoveLayerDown(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto IsTileLayerActive(const entt::registry& registry) -> bool;

[[nodiscard]] auto GetActiveLayerID(const entt::registry& registry) -> Maybe<LayerID>;

inline void RemoveLayer(entt::registry& registry, const LayerID id)
{
  const auto entity = FindLayer(registry, id);
  RemoveLayer(registry, entity);
}

inline void SelectLayer(entt::registry& registry, const LayerID id)
{
  const auto entity = FindLayer(registry, id);
  SelectLayer(registry, entity);
}

inline auto DuplicateLayer(entt::registry& registry, const LayerID id) -> entt::entity
{
  const auto entity = FindLayer(registry, id);
  return DuplicateLayer(registry, entity);
}

inline void MoveLayerUp(entt::registry& registry, const LayerID id)
{
  const auto entity = FindLayer(registry, id);
  MoveLayerUp(registry, entity);
}

inline void MoveLayerDown(entt::registry& registry, const LayerID id)
{
  const auto entity = FindLayer(registry, id);
  MoveLayerDown(registry, entity);
}

inline void SetLayerOpacity(entt::registry& registry,
                            const LayerID id,
                            const float opacity)
{
  const auto entity = FindLayer(registry, id);
  SetLayerOpacity(registry, entity, opacity);
}

inline void SetLayerVisible(entt::registry& registry,
                            const LayerID id,
                            const bool visible)
{
  const auto entity = FindLayer(registry, id);
  SetLayerVisible(registry, entity, visible);
}

[[nodiscard]] inline auto GetLayerOpacity(const entt::registry& registry,
                                          const LayerID id) -> float
{
  const auto entity = FindLayer(registry, id);
  return GetLayerOpacity(registry, entity);
}

[[nodiscard]] inline auto IsLayerVisible(const entt::registry& registry, const LayerID id)
    -> bool
{
  const auto entity = FindLayer(registry, id);
  return IsLayerVisible(registry, entity);
}

[[nodiscard]] inline auto CanMoveLayerUp(const entt::registry& registry, const LayerID id)
    -> bool
{
  const auto entity = FindLayer(registry, id);
  return CanMoveLayerUp(registry, entity);
}

[[nodiscard]] inline auto CanMoveLayerDown(const entt::registry& registry,
                                           const LayerID id) -> bool
{
  const auto entity = FindLayer(registry, id);
  return CanMoveLayerDown(registry, entity);
}

/// \} End of layer system

}  // namespace Tactile::Sys
