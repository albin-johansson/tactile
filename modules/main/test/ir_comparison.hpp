// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/io/ir.hpp"

namespace tactile::test {

void expect_eq(const ir::AttachedComponent& component1,
               const ir::AttachedComponent& component2);

void expect_eq(const ir::Component& component1, const ir::Component& component2);

void expect_eq(const ir::Metadata& metadata1, const ir::Metadata& metadata2);

void expect_eq(const ir::Object& object1, const ir::Object& object2);

void expect_eq(const ir::Layer& layer1, const ir::Layer& layer2);

void expect_eq(const ir::TileFormat& format1, const ir::TileFormat& format2);

void expect_eq(const ir::AnimationFrame& frame1, const ir::AnimationFrame& frame2);

void expect_eq(const ir::Tile& tile1, const ir::Tile& tile2);

void expect_eq(const ir::Tileset& tileset1, const ir::Tileset& tileset2);

void expect_eq(const ir::TilesetRef& tileset_ref1, const ir::TilesetRef& tileset_ref2);

void expect_eq(const ir::Map& map1, const ir::Map& map2);

}  // namespace tactile::test
