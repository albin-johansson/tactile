// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <chrono>    // milliseconds
#include <expected>  // expected
#include <utility>   // pair

#include "tactile/base/debug/error_code.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IDocumentVisitor;
class ITilesetView;
class IMetaView;

/**
 * A read-only view of a tile in a tileset.
 */
class ITileView
{
 public:
  TACTILE_INTERFACE_CLASS(ITileView);

  /**
   * Inspects the tile.
   *
   * \param visitor The visitor to use.
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto accept(IDocumentVisitor& visitor) const -> std::expected<void, ErrorCode> = 0;

  /**
   * Returns the tileset that hosts the tile.
   *
   * \return
   * A tileset view.
   */
  [[nodiscard]]
  virtual auto get_parent_tileset() const -> const ITilesetView& = 0;

  /**
   * Returns the local identifier associated with the tile.
   *
   * \return
   * A tile index.
   */
  [[nodiscard]]
  virtual auto get_index() const -> TileIndex = 0;

  /**
   * Returns the number of objects stored in the tile.
   *
   * \return
   * An object count.
   */
  [[nodiscard]]
  virtual auto object_count() const -> std::size_t = 0;

  /**
   * Returns the number of frames in the associated animation, if any.
   *
   * \return
   * An animation frame count.
   */
  [[nodiscard]]
  virtual auto animation_frame_count() const -> std::size_t = 0;

  /**
   * Returns the frame at a given index in the associated animation.
   *
   * \param index The animation frame index.
   *
   * \return
   * A tile index and frame duration pair.
   */
  [[nodiscard]]
  virtual auto get_animation_frame(std::size_t index) const
      -> std::pair<TileIndex, std::chrono::milliseconds> = 0;

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
