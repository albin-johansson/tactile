// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <optional>      // optional
#include <system_error>  // error_code

#include "tactile/base/container/path.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/io/compress/compression_format.hpp"
#include "tactile/base/layer/tile_encoding.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"

namespace tactile {

class IDocumentVisitor;
class IMetaView;

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
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  virtual auto accept(IDocumentVisitor& visitor) const
      -> std::expected<void, std::error_code> = 0;

  /**
   * Returns the file path of the associated save file, if any.
   *
   * \return
   * A possibly null file path.
   */
  [[nodiscard]]
  virtual auto get_path() const -> const Path* = 0;

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
   * Returns the tile encoding format used by the map.
   *
   * \return
   * A tile encoding format.
   */
  [[nodiscard]]
  virtual auto get_tile_encoding() const -> TileEncoding = 0;

  /**
   * Returns the tile compression format used by the map, if any.
   *
   * \return
   * A compression format if compression is used; an empty optional otherwise.
   */
  [[nodiscard]]
  virtual auto get_tile_compression() const -> std::optional<CompressionFormat> = 0;

  /**
   * Returns the compression level used by the map, if any.
   *
   * \return
   * A compression level if compression is used; an empty optional otherwise.
   */
  [[nodiscard]]
  virtual auto get_compression_level() const -> std::optional<int> = 0;

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

  /**
   * Returns a view of the associated metadata.
   *
   * \return
   * A metadata view.
   */
  [[nodiscard]]
  virtual auto get_meta() const -> const IMetaView& = 0;
};

}  // namespace tactile
