// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/context/meta_context.hpp"
#include "tactile/core/context/metadata.hpp"
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
 * \brief Provides information needed to construct a tileset.
 */
struct TilesetCreateInfo final {
  ssize tile_count;      ///< The total number of tiles.
  ssize column_count;    ///< The number of tile columns.
  Int2 tile_size;        ///< The dimensions of each tile.
  Int2 texture_size;     ///< The dimensions of the tileset image.
  TextureID texture_id;  ///< The ID of the tileset image.
};

/**
 * \brief Represents a collection of tiles contained in a single image.
 */
class TACTILE_CORE_API Tileset final : public IMetaContext {
 public:
  /**
   * \brief Creates a tileset.
   *
   * \param info the tileset configuration.
   */
  explicit Tileset(const TilesetCreateInfo& info);

  void accept(IMetaContextVisitor& visitor) override;

  /**
   * \brief Updates tile animations and resets internal caches.
   *
   * \details This function should be called once per frame.
   */
  void update();

  /**
   * \brief Returns the tile index corresponding to a given tile position.
   *
   * \param pos the tile position.
   *
   * \return a tile index.
   */
  [[nodiscard]]
  auto index_of(const TilePos& pos) const -> Maybe<TileIndex>;

  /**
   * \brief Returns the displayed appearance of a tile.
   *
   * \note This function caches the results from each invocation. Make sure to reset the
   *       cache via `update()` at the start of each frame.
   *
   * \param tile_index a tile index, must be valid.
   *
   * \return the index of the tile that should be rendered.
   */
  [[nodiscard]]
  auto get_appearance(TileIndex tile_index) const -> TileIndex;

  /**
   * \brief Returns the tile at the specified index, if any.
   *
   * \param tile_index the index of the desired tile.
   *
   * \return a pointer to the found tile.
   */
  [[nodiscard]]
  auto find_tile(TileIndex tile_index) -> Tile*;

  /**
   * \copydoc find_tile()
   */
  [[nodiscard]]
  auto find_tile(TileIndex tile_index) const -> const Tile*;

  /**
   * \brief Returns the tile at the specified index.
   *
   * \param tile_index the index of the desired tile.
   *
   * \return the found tile.
   */
  [[nodiscard]]
  auto get_tile(TileIndex tile_index) -> Tile&;

  /**
   * \copydoc get_tile()
   */
  [[nodiscard]]
  auto get_tile(TileIndex tile_index) const -> const Tile&;

  /**
   * \brief Indicates whether a tile index references a valid tile in the tileset.
   *
   * \param tile_index the tile index.
   *
   * \return true if the index is valid; false otherwise.
   */
  [[nodiscard]]
  auto is_valid_index(TileIndex tile_index) const -> bool;

  /**
   * \brief Returns the last valid tile index in the tileset.
   *
   * \return a tile index.
   */
  [[nodiscard]]
  auto last_tile_index() const -> TileIndex;

  /**
   * \brief Returns the number of tiles in the tileset.
   *
   * \return a tile count.
   */
  [[nodiscard]]
  auto tile_count() const -> ssize;

  /**
   * \brief Returns the number of tile rows in the tileset.
   *
   * \return a row count.
   */
  [[nodiscard]]
  auto row_count() const -> ssize;

  /**
   * \brief Returns the number of tile columns in the tileset.
   *
   * \return a column count.
   */
  [[nodiscard]]
  auto column_count() const -> ssize;

  /**
   * \brief Returns the size of each tile in the tileset.
   *
   * \return a tile size.
   */
  [[nodiscard]]
  auto get_tile_size() const -> Int2;

  /**
   * \brief Returns the normalized size of each tile in the tileset.
   *
   * \return a normalized tile size.
   */
  [[nodiscard]]
  auto get_normalized_tile_size() const -> Float2;

  /**
   * \brief Returns the size of the associated texture.
   *
   * \return a texture size.
   */
  [[nodiscard]]
  auto get_texture_size() const -> Int2;

  /**
   * \brief Returns the ID of the associated texture.
   *
   * \return a texture identifier.
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