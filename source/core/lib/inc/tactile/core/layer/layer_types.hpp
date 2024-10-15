// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>     // int32_t
#include <functional>  // less
#include <map>         // map
#include <optional>    // optional
#include <string>      // string
#include <vector>      // vector

#include "tactile/base/id.hpp"
#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/numeric/extent_2d.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/util/tile_matrix.hpp"

namespace tactile::core {

/**
 * A component that provides common layer information.
 */
struct CLayer final
{
  /** The associated identifier used in save files. */
  std::optional<std::int32_t> persistent_id;

  /** The opacity of the layer when rendered. */
  float opacity;

  /** Indicates whether the layer is rendered. */
  bool visible;
};

/**
 * A component that represents a layer variant that stores other layers.
 */
struct CGroupLayer final
{
  /** The stored layers. */
  std::vector<EntityID> layers;
};

using SparseTileMatrix = std::map<Index2D, TileID, std::less<>>;

/**
 * Base component for tile layers.
 */
struct CTileLayer final
{
  Extent2D extent;
};

/**
 * Component for densely populated tile layers.
 */
struct CDenseTileLayer final
{
  /** The associated tile data. */
  TileMatrix tiles;
};

/**
 * Component for sparsely populated tile layers.
 */
struct CSparseTileLayer final
{
  /** The associated tile data. */
  SparseTileMatrix tiles;
};

/**
 * A component that represents a layer of objects.
 */
struct CObjectLayer final
{
  /** The associated objects. */
  std::vector<EntityID> objects;
};

/**
 * A component for layer objects.
 */
struct CObject final
{
  /** The associated identifier. */
  ObjectID id;

  /** The object position. */
  Float2 position;

  /** The object size. */
  Float2 size;

  /** The object type. */
  ObjectType type;

  /** Arbitrary user-provided tag. */
  std::string tag;

  /** Indicates whether the object is rendered. */
  bool is_visible;
};

}  // namespace tactile::core
