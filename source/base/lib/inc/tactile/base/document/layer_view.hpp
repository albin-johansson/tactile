// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <optional>      // optional
#include <system_error>  // error_code

#include "tactile/base/id.hpp"
#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/io/compress/compression_format.hpp"
#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/layer/tile_encoding.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"
#include "tactile/base/util/matrix_index.hpp"

namespace tactile {

class IDocumentVisitor;
class IMetaView;

/**
 * A read-only view of a layer.
 */
class ILayerView
{
 public:
  TACTILE_INTERFACE_CLASS(ILayerView);

  /**
   * Inspects the layer.
   *
   * \param visitor The visitor to use.
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto accept(IDocumentVisitor& visitor) const
      -> std::expected<void, std::error_code> = 0;

  virtual void write_tile_bytes(ByteStream& byte_stream) const = 0;

  /**
   * Returns a view of the parent layer, if any.
   *
   * \return
   * A group layer view if the layer is hosted; a null pointer otherwise.
   */
  [[nodiscard]]
  virtual auto get_parent_layer() const -> const ILayerView* = 0;

  /**
   * Returns the persistent identifier associated with the layer.
   *
   * \return
   * A layer identifier.
   */
  [[nodiscard]]
  virtual auto get_id() const -> LayerID = 0;

  /**
   * Returns the type of the layer.
   *
   * \return
   * A layer type.
   */
  [[nodiscard]]
  virtual auto get_type() const -> LayerType = 0;

  /**
   * Returns the opacity of the layer.
   *
   * \return
   * An opacity value, in the interval [0, 1].
   */
  [[nodiscard]]
  virtual auto get_opacity() const -> float = 0;

  /**
   * Indicates whether the layer is visible.
   *
   * \return
   * True if the layer is rendered; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_visible() const -> bool = 0;

  /**
   * Returns the global index of the layer in the root layer hierarchy.
   *
   * \return
   * A global layer index.
   */
  [[nodiscard]]
  virtual auto get_global_index() const -> usize = 0;

  /**
   * Returns the number of contained layers (for group layers).
   *
   * \return
   * A layer count.
   */
  [[nodiscard]]
  virtual auto layer_count() const -> usize = 0;

  /**
   * Returns the number of contained objects (for object layers).
   *
   * \return
   * An object count.
   */
  [[nodiscard]]
  virtual auto object_count() const -> usize = 0;

  /**
   * Returns a tile in the associated tile layer.
   *
   * \note
   * This function is only useful for tile layer views.
   *
   * \param index The tile index.
   *
   * \return
   * The identifier of the specified tile if the layer is a tile layer; an empty
   * optional otherwise.
   */
  [[nodiscard]]
  virtual auto get_tile(const MatrixIndex& index) const -> std::optional<TileID> = 0;

  /**
   * Returns the tile encoding format used by the layer.
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
   * Returns the extent of the associated tile layer.
   *
   * \return
   * The extent of the layer if it's a tile layer; an empty optional otherwise.
   */
  [[nodiscard]]
  virtual auto get_extent() const -> std::optional<MatrixExtent> = 0;

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
