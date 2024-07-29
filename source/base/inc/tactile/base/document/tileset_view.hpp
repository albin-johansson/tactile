// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IDocumentVisitor;
class ITileView;
class IMetaView;

/**
 * A read-only view of a tileset.
 */
class ITilesetView
{
 public:
  TACTILE_INTERFACE_CLASS(ITilesetView);

  /**
   * Inspects the tileset.
   *
   * \param visitor The visitor to use.
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto accept(IDocumentVisitor& visitor) const -> Result<void> = 0;

  /**
   * Returns the first global tile identifier associated with the tileset.
   *
   * \return
   * A tile identifier.
   */
  [[nodiscard]]
  virtual auto get_first_tile_id() const -> TileID = 0;

  /**
   * Returns the total number of tiles in the tileset.
   *
   * \note
   * The value returned by this doesn't necessarily reflect the number of
   * available tile definition views.
   *
   * \return
   * A tile count.
   */
  [[nodiscard]]
  virtual auto tile_count() const -> usize = 0;

  /**
   * Returns the number of tile definitions in the tileset.
   *
   * \return
   * A tile definition count.
   */
  [[nodiscard]]
  virtual auto tile_definition_count() const -> usize = 0;

  /**
   * Returns the number of columns in the tileset.
   *
   * \return
   * A column count.
   */
  [[nodiscard]]
  virtual auto column_count() const -> usize = 0;

  /**
   * Returns the logical size of tiles in the tileset.
   *
   * \return
   * A tile size.
   */
  [[nodiscard]]
  virtual auto get_tile_size() const -> Int2 = 0;

  /**
   * Returns the size of the associated image.
   *
   * \return
   * An image size.
   */
  [[nodiscard]]
  virtual auto get_image_size() const -> Int2 = 0;

  /**
   * Returns the file path to the associated image.
   *
   * \return
   * An image path.
   */
  [[nodiscard]]
  virtual auto get_image_path() const -> const Path& = 0;

  /**
   * Returns a view of the associated metadata.
   *
   * \return
   * A metadata view.
   */
  [[nodiscard]]
  virtual auto get_meta() const -> const IMetaView& = 0;

  /**
   * Returns the filename of the associated external tileset file (excluding the extension).
   *
   * \return
   * A filename without a file extension.
   */
  [[nodiscard]]
  virtual auto get_filename() const -> String = 0;
};

}  // namespace tactile
