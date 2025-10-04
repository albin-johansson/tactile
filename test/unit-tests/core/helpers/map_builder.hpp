// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

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
