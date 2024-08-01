// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::test {

enum class AttributeEqMode
{
  kTactile,
  kTiled
};

void expect_eq(const ir::NamedAttribute& attr1,
               const ir::NamedAttribute& attr2,
               AttributeEqMode mode);

void expect_eq(const ir::AttachedComponent& component1,
               const ir::AttachedComponent& component2,
               AttributeEqMode mode);

void expect_eq(const ir::Component& component1,
               const ir::Component& component2,
               AttributeEqMode mode);

void expect_eq(const ir::Metadata& meta1, const ir::Metadata& meta2, AttributeEqMode mode);

void expect_eq(const ir::Object& object1, const ir::Object& object2, AttributeEqMode mode);

void expect_eq(const ir::Layer& layer1, const ir::Layer& layer2, AttributeEqMode mode);

void expect_eq(const ir::Tile& tile1, const ir::Tile& tile2, AttributeEqMode mode);

void expect_eq(const ir::Tileset& tileset1, const ir::Tileset& tileset2, AttributeEqMode mode);

void expect_eq(const ir::TilesetRef& tileset_ref1,
               const ir::TilesetRef& tileset_ref2,
               AttributeEqMode mode);

void expect_eq(const ir::Map& map1, const ir::Map& map2, AttributeEqMode mode);

}  // namespace tactile::test
