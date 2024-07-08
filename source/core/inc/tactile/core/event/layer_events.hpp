// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/**
 * Event for creating a new layer in the active map.
 */
struct CreateLayerEvent final
{
  /** The type of layer to create. */
  LayerType type;
};

/**
 * Event for removing a layer from the active map.
 */
struct RemoveLayerEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for duplicating a layer in the active map.
 */
struct DuplicateLayerEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for moving a layer up in the active map.
 */
struct MoveLayerUpEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for moving a layer down in the active map.
 */
struct MoveLayerDownEvent final
{
  /** The target layer. */
  EntityID layer_entity;
};

/**
 * Event for changing the name of a layer in the active map.
 */
struct RenameLayerEvent final
{
  /** The target layer. */
  EntityID layer_entity;

  /** The new layer name. */
  String name;
};

/**
 * Event for changing the opacity of a layer in the active map.
 */
struct SetLayerOpacityEvent final
{
  /** The target layer. */
  EntityID layer_entity;

  /** The new layer opacity. */
  float opacity;
};

/**
 * Event for changing the visibility of a layer in the active map.
 */
struct SetLayerVisibleEvent final
{
  /** The target layer. */
  EntityID layer_entity;

  /** The new visibility. */
  bool visible;
};

}  // namespace tactile
