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

#include "core/attributes/attribute.hpp"
#include "core/tiles/tile_extent.hpp"
#include "core/tiles/tile_pos.hpp"
#include "core/ecs/entity.hpp"
#include "core/functional/maybe.hpp"
#include "core/containers/string.hpp"
#include "core/containers/vector.hpp"
#include "core/prelude.hpp"
#include "io/ir/map/map_ir.hpp"
#include "model/registry.hpp"

namespace tactile {

class LayerBuilder final {
 public:
  using Self = LayerBuilder;

  explicit LayerBuilder(Registry& registry);

  auto with_id(int32 id) -> Self&;

  auto with_name(String name) -> Self&;

  auto with_opacity(float opacity) -> Self&;

  auto with_visibility(bool visible) -> Self&;

  auto with_property(String name, Attribute value) -> Self&;

  auto with_component(Entity component_entity) -> Self&;

  // Tile layer only
  auto as_tile_layer(TileExtent extent) -> Self&;
  auto set_tile(TilePos pos, TileID tile_id) -> Self&;

  // Object layer only
  auto as_object_layer() -> Self&;
  auto with_object(int32 id, ObjectType type, Entity* object_entity = nullptr) -> Self&;

  // Group layer only
  auto as_group_layer() -> Self&;
  auto with_layer(Entity layer_entity) -> Self&;

  [[nodiscard]] auto build() -> Entity;

 private:
  Registry* mRegistry;
  String mName;
  int32 mId {};
  AttributeMap mProperties;
  Vector<Entity> mComponents;
  Maybe<TileMatrix> mTiles;
  Maybe<Vector<Entity>> mObjects;
  Maybe<Vector<Entity>> mLayers;
  float mOpacity {1.0f};
  bool mVisible {true};
};

}  // namespace tactile
