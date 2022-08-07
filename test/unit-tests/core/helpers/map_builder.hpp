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

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/macros.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/document/map_document.hpp"
#include "core/layer/object.hpp"
#include "core/map.hpp"

namespace tactile::test {

class MapBuilder final
{
 public:
  TACTILE_DELETE_COPY(MapBuilder);
  TACTILE_DEFAULT_MOVE(MapBuilder);

  [[nodiscard]] static auto build() -> MapBuilder;

  auto with_tile_size(const Vector2i& size) -> MapBuilder&;

  auto with_size(usize rows, usize columns) -> MapBuilder&;

  auto with_tile_layer(UUID* id = nullptr, Maybe<TileID> initialValue = nothing)
      -> MapBuilder&;

  auto with_object_layer(UUID* id = nullptr) -> MapBuilder&;

  auto with_object(ObjectType type, Shared<Object>* outObject = nullptr) -> MapBuilder&;

  auto with_tileset(UUID* id = nullptr) -> MapBuilder&;

  [[nodiscard]] auto result() -> Unique<MapDocument>;

 private:
  Unique<MapDocument> mDocument;
  Maybe<UUID>         mDedicatedObjectLayer;

  MapBuilder();
};

}  // namespace tactile::test
