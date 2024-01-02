// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class OrthogonalMap;
class IsometricMap;
class HexagonalMap;
class GroupLayer;
class TileLayer;
class ObjectLayer;
class Object;
class Tileset;
class Tile;

/**
 * Interface for meta context visitors.
 */
class IMetaContextVisitor {
 public:
  TACTILE_INTERFACE_CLASS(IMetaContextVisitor);

  /**
   * Visits an orthogonal map.
   *
   * \param map The map to visit.
   */
  virtual void visit([[maybe_unused]] OrthogonalMap& map) {};

  /**
   * Visits an isometric map.
   *
   * \param map The map to visit.
   */
  virtual void visit([[maybe_unused]] IsometricMap& map) {};

  /**
   * Visits a hexagonal map.
   *
   * \param map The map to visit.
   */
  virtual void visit([[maybe_unused]] HexagonalMap& map) {};

  /**
   * Visits a group layer.
   *
   * \param layer The layer to visit.
   */
  virtual void visit([[maybe_unused]] GroupLayer& layer) {};

  /**
   * Visits a tile layer.
   *
   * \param layer The layer to visit.
   */
  virtual void visit([[maybe_unused]] TileLayer& layer) {};

  /**
   * Visits an object layer.
   *
   * \param layer The layer to visit.
   */
  virtual void visit([[maybe_unused]] ObjectLayer& layer) {};

  /**
   * Visits an object.
   *
   * \param object The object to visit.
   */
  virtual void visit([[maybe_unused]] Object& object) {};

  /**
   * Visits a tileset.
   *
   * \param tileset The tileset to visit.
   */
  virtual void visit([[maybe_unused]] Tileset& tileset) {};

  /**
   * Visits a tile.
   *
   * \param tile The tile to visit.
   */
  virtual void visit([[maybe_unused]] Tile& tile) {};
};

}  // namespace tactile
