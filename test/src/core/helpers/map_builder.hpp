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

#include "core/layer/object.hpp"
#include "core/map.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/ptr.hpp"
#include "core/vocabulary.hpp"
#include "model/document/map_document.hpp"

namespace tactile::test {

class MapBuilder final {
 public:
  TACTILE_DELETE_COPY(MapBuilder);
  TACTILE_DEFAULT_MOVE(MapBuilder);

  [[nodiscard]] static auto build() -> MapBuilder;

  auto with_tile_size(const Int2& size) -> MapBuilder&;

  auto with_size(usize rows, usize columns) -> MapBuilder&;

  auto with_tile_layer(UUID* id = nullptr, Maybe<TileID> initial_value = nothing)
      -> MapBuilder&;

  auto with_object_layer(UUID* id = nullptr) -> MapBuilder&;

  auto with_object(ObjectType type,
                   Shared<Object>* out_object = nullptr,
                   UUID* out_layer = nullptr) -> MapBuilder&;

  auto with_tileset(UUID* id = nullptr) -> MapBuilder&;

  [[nodiscard]] auto result() -> Unique<MapDocument>;

 private:
  Unique<MapDocument> mDocument;
  Maybe<UUID> mDedicatedObjectLayer;

  MapBuilder();
};

}  // namespace tactile::test
