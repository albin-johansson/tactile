// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/core/meta/metadata.hpp"
#include "tactile/core/tileset/tile.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/misc/tile_pos.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Provides information needed to construct a tileset.
 */
struct TilesetCreateInfo final {
  ssize tile_count;      ///< The total number of tiles.
  ssize column_count;    ///< The number of tile columns.
  Int2 tile_size;        ///< The dimensions of each tile.
  Int2 texture_size;     ///< The dimensions of the tileset image.
  TextureID texture_id;  ///< The ID of the tileset image.
};

/**
 * Represents a collection of tiles contained in a single image.
 */
class TACTILE_CORE_API Tileset final : public IMetaContext {
 public:
  /**
   * Creates a tileset.
   *
   * \param info The tileset configuration.
   */
  explicit Tileset(const TilesetCreateInfo& info);

  void accept(IMetaContextVisitor& visitor) override;

  /**
   * Updates tile animations and resets internal caches.
   *
   * \details
   *    This function should be called once per frame.
   */
  void update();

  /**
   * Returns the tile index corresponding to a given tile position.
   *
   * \param pos The tile position.
   *
   * \return
   *    A tile index.
   */
  [[nodiscard]]
  auto index_of(const TilePos& pos) const -> Maybe<TileIndex>;

  /**
   * Returns the displayed appearance of a tile.
   *
   * \note
   *    This function caches the results from each invocation. Make sure to
   *    reset the cache via `update()` at the start of each frame.
   *
   * \param tile_index A tile index, must be valid.
   *
   * \return
   *    The index of the tile that should be rendered.
   */
  [[nodiscard]]
  auto get_appearance(TileIndex tile_index) const -> TileIndex;

  /**
   * Returns the tile at the specified index, if any.
   *
   * \param tile_index The index of the desired tile.
   *
   * \return
   *    A pointer to the found tile.
   */
  [[nodiscard]]
  auto find_tile(TileIndex tile_index) -> Tile*;

  /** \copydoc find_tile() */
  [[nodiscard]]
  auto find_tile(TileIndex tile_index) const -> const Tile*;

  /**
   * Returns the tile at the specified index.
   *
   * \param tile_index The index of the desired tile.
   *
   * \return
   *    The found tile.
   */
  [[nodiscard]]
  auto get_tile(TileIndex tile_index) -> Tile&;

  /** \copydoc get_tile() */
  [[nodiscard]]
  auto get_tile(TileIndex tile_index) const -> const Tile&;

  /**
   * Indicates whether a tile index references a valid tile in the tileset.
   *
   * \param tile_index The tile index.
   *
   * \return
   *    True if the index is valid; false otherwise.
   */
  [[nodiscard]]
  auto is_valid_index(TileIndex tile_index) const -> bool;

  /**
   * Returns the last valid tile index in the tileset.
   *
   * \return
   *    A tile index.
   */
  [[nodiscard]]
  auto last_tile_index() const -> TileIndex;

  /**
   * Returns the number of tiles in the tileset.
   *
   * \return
   *    A tile count.
   */
  [[nodiscard]]
  auto tile_count() const -> ssize;

  /**
   * Returns the number of tile rows in the tileset.
   *
   * \return
   *    A row count.
   */
  [[nodiscard]]
  auto row_count() const -> ssize;

  /**
   * Returns the number of tile columns in the tileset.
   *
   * \return
   *    A column count.
   */
  [[nodiscard]]
  auto column_count() const -> ssize;

  /**
   * Returns the size of each tile in the tileset.
   *
   * \return
   *    A tile size.
   */
  [[nodiscard]]
  auto get_tile_size() const -> Int2;

  /**
   * Returns the normalized size of each tile in the tileset.
   *
   * \return
   *    A normalized tile size.
   */
  [[nodiscard]]
  auto get_normalized_tile_size() const -> Float2;

  /**
   * Returns the size of the associated texture.
   *
   * \return
   *    A texture size.
   */
  [[nodiscard]]
  auto get_texture_size() const -> Int2;

  /**
   * Returns the ID of the associated texture.
   *
   * \return
   *    A texture identifier.
   */
  [[nodiscard]]
  auto get_texture_id() const -> TextureID;

  [[nodiscard]]
  auto meta() -> Metadata& override;

  [[nodiscard]]
  auto meta() const -> const Metadata& override;

  [[nodiscard]] auto begin() const { return mTiles.begin(); }

  [[nodiscard]] auto end() const { return mTiles.end(); }

 private:
  Metadata mMeta;
  ssize mTileCount;
  ssize mColumnCount;
  Int2 mTileSize;
  Int2 mTextureSize;
  Float2 mNormalizedTileSize;
  TextureID mTextureId;
  Vector<Tile> mTiles;
  mutable Vector<Maybe<TileIndex>> mAppearanceCache;
  FilePath mImagePath;

  void _create_tiles();
};

}  // namespace tactile