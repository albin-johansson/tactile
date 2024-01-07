// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/meta/meta_context_visitor.hpp"

namespace tactile::testutil {

class MetaContextVisitorTester final : public IMetaContextVisitor {
 public:
  int orthogonal_map_count {0};
  int group_layer_count {0};
  int tile_layer_count {0};
  int object_layer_count {0};
  int object_count {0};
  int tileset_count {0};
  int tile_count {0};

  void visit([[maybe_unused]] OrthogonalMap& map) override { ++orthogonal_map_count; }

  void visit([[maybe_unused]] GroupLayer& layer) override { ++group_layer_count; }

  void visit([[maybe_unused]] TileLayer& layer) override { ++tile_layer_count; }

  void visit([[maybe_unused]] ObjectLayer& layer) override { ++object_layer_count; }

  void visit([[maybe_unused]] Object& object) override { ++object_count; }

  void visit([[maybe_unused]] Tileset& tileset) override { ++tileset_count; }

  void visit([[maybe_unused]] Tile& tile) override { ++tile_count; }
};

}  // namespace tactile::testutil
