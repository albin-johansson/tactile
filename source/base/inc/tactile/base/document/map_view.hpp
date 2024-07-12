// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"

namespace tactile {

class IDocumentVisitor;

/**
 * A read-only view of a map.
 */
class IMapView
{
 public:
  TACTILE_INTERFACE_CLASS(IMapView);

  /**
   * Inspects the map.
   *
   * \param visitor The visitor to use.
   */
  virtual void accept(IDocumentVisitor& visitor) const = 0;

  /**
   * Returns the logical size of tiles in the map.
   *
   * \return
   * A tile size.
   */
  [[nodiscard]]
  virtual auto get_tile_size() const -> Int2 = 0;

  /**
   * Returns the extent of the map.
   *
   * \return
   * A map extent.
   */
  [[nodiscard]]
  virtual auto get_extent() const -> MatrixExtent = 0;

  /**
   * Returns the next available layer identifier.
   *
   * \return
   * A layer identifier.
   */
  [[nodiscard]]
  virtual auto get_next_layer_id() const -> LayerID = 0;

  /**
   * Returns the next available object identifier.
   *
   * \return
   * An object identifier.
   */
  [[nodiscard]]
  virtual auto get_next_object_id() const -> ObjectID = 0;

  /**
   * Returns the number of layers in the map.
   *
   * \return
   * A layer count.
   */
  [[nodiscard]]
  virtual auto layer_count() const -> usize = 0;

  /**
   * Returns the number of tilesets attached to the map.
   *
   * \return
   * A tileset count.
   */
  [[nodiscard]]
  virtual auto tileset_count() const -> usize = 0;

  /**
   * Returns the number of component definitions in the map.
   *
   * \return
   * A component count.
   */
  [[nodiscard]]
  virtual auto component_count() const -> usize = 0;
};

}  // namespace tactile
