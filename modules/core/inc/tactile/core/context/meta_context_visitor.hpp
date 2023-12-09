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
 * \interface IMetaContextVisitor
 * \brief Interface for meta context visitors.
 */
class IMetaContextVisitor {
 public:
  TACTILE_INTERFACE_CLASS(IMetaContextVisitor);

  /**
   * \brief Visits an orthogonal map.
   *
   * \param map the map to visit.
   */
  virtual void visit([[maybe_unused]] OrthogonalMap& map) {};

  /**
   * \brief Visits an isometric map.
   *
   * \param map the map to visit.
   */
  virtual void visit([[maybe_unused]] IsometricMap& map) {};

  /**
   * \brief Visits a hexagonal map.
   *
   * \param map the map to visit.
   */
  virtual void visit([[maybe_unused]] HexagonalMap& map) {};

  /**
   * \brief Visits a group layer.
   *
   * \param layer the layer to visit.
   */
  virtual void visit([[maybe_unused]] GroupLayer& layer) {};

  /**
   * \brief Visits a tile layer.
   *
   * \param layer the layer to visit.
   */
  virtual void visit([[maybe_unused]] TileLayer& layer) {};

  /**
   * \brief Visits an object layer.
   *
   * \param layer the layer to visit.
   */
  virtual void visit([[maybe_unused]] ObjectLayer& layer) {};

  /**
   * \brief Visits an object.
   *
   * \param object the object to visit.
   */
  virtual void visit([[maybe_unused]] Object& object) {};

  /**
   * \brief Visits a tileset.
   *
   * \param tileset the tileset to visit.
   */
  virtual void visit([[maybe_unused]] Tileset& tileset) {};

  /**
   * \brief Visits a tile.
   *
   * \param tile the tile to visit.
   */
  virtual void visit([[maybe_unused]] Tile& tile) {};
};

}  // namespace tactile
