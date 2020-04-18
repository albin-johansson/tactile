#pragma once

#include <set>
#include <type_traits>

#include "smart_pointers.h"
#include "tile_id.h"

class QImage;

namespace tactile {

/**
 * The <code>TileSheet</code> class represents an image that contains a set
 * of tiles that are used to build tile maps. All tile sheets store their
 * first and last valid tile ID. Tile sheets should be created from images
 * that store their sprites aligned in a grid. Tiles in a tile sheet don't
 * need to be square.
 *
 * @since 0.1.0
 */
class TileSheet final { // TODO rename to TileSet
 public:
  /**
   * Creates a tile sheet with the initial first ID set to 1. Both the
   * supplied width and height will be adjusted to be at least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tile sheet.
   * @param tileHeight the height of the tiles in the tile sheet.
   * @throws BadArg if the supplied image is null.
   * @since 0.1.0
   */
  TileSheet(const Shared<QImage>& image, int tileWidth, int tileHeight);

  TileSheet(const TileSheet& other) noexcept;

  TileSheet(TileSheet&& other) noexcept;

  ~TileSheet() noexcept;

  /**
   * Creates and returns a unique pointer to a tile sheet instance. Both the
   * supplied width and height will be adjusted to be at least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tile sheet.
   * @param tileHeight the height of the tiles in the tile sheet.
   * @throws invalid_argument if the supplied image is null.
   * @return a unique pointer to a tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] static Unique<TileSheet> unique(const Shared<QImage>& image,
                                                int tileWidth,
                                                int tileHeight);

  /**
   * Creates and returns a shared pointer to a tile sheet instance. Both the
   * supplied width and height will be adjusted to be at least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tile sheet.
   * @param tileHeight the height of the tiles in the tile sheet.
   * @throws invalid_argument if the supplied image is null.
   * @return a shared pointer to a tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] static Shared<TileSheet> shared(const Shared<QImage>& image,
                                                int tileWidth,
                                                int tileHeight);

  /**
   * Sets the first tile ID property of the tile sheet. Note that this method
   * doesn't check if the supplied tile ID isn't already taken by another
   * tile sheet. This method has no effect if the supplied ID isn't greater than
   * zero.
   *
   * @param firstID the new first tile ID, must be greater than zero.
   * @since 0.1.0
   */
  void set_first_id(TileID firstID) noexcept;

  /**
   * Selects the tile at the specified coordinates. Multiple tiles can be
   * selected simultaneously. This method has no effect if the supplied
   * position is out-of-bounds.
   *
   * @param x the x-coordinate of the selection.
   * @param y the y-coordinate of the selection.
   * @since 0.1.0
   */
  void select(int x, int y) noexcept;

  /**
   * Clears any previously selected tiles.
   *
   * @since 0.1.0
   */
  void clear_selection() noexcept;

  /**
   * Indicates whether or not the tile sheet contains the specified tile ID.
   *
   * @param id the tile ID that will be checked.
   * @return true if the tile sheet contains the tile ID; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool contains(TileID id) const noexcept;

  /**
   * Returns the tile ID of the tile at the specified coordinates.
   *
   * @param x the x-coordinate to check.
   * @param y the y-coordinate to check.
   * @return the tile ID of the tile at the specified position;
   * <code>empty</code> if no tile was found.
   * @since 0.1.0
   */
  [[nodiscard]] TileID tile_at(int x, int y) const noexcept;

  /**
   * Returns the width of the tile sheet image.
   *
   * @return the width of the tile sheet image.
   * @since 0.1.0
   */
  [[nodiscard]] int width() const noexcept;

  /**
   * Returns the height of the tile sheet image.
   *
   * @return the height of the tile sheet image.
   * @since 0.1.0
   */
  [[nodiscard]] int height() const noexcept;

  /**
   * Returns the total number of rows of tiles in the tile sheet.
   *
   * @return the total number of rows in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept { return m_rows; }

  /**
   * Returns the total number of columns of tiles in the tile sheet.
   *
   * @return the total number of columns in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept { return m_cols; }

  /**
   * Returns the total number of tiles contained in the tile sheet.
   *
   * @return the total number of tiles contained in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] int tiles() const noexcept { return m_nTiles; }

  /**
   * Returns the tile ID of the first tile in the tile sheet. The default
   * first tile ID is 1.
   *
   * @return the tile ID of the first tile in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] TileID first_id() const noexcept { return m_firstID; }

  /**
   * Returns the last valid tile ID associated with the tile sheet.
   *
   * @return the last valid tile ID associated with the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] TileID last_id() const noexcept { return first_id() + tiles(); }

  /**
   * Returns the width of the tile sprites in the tile sheet. Will always be
   * at least 1.
   *
   * @return the width of the tile sprites in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] int tile_width() const noexcept { return m_tileWidth; }

  /**
   * Returns the height of the tile sprites in the tile sheet. Will always be
   * at least 1.
   *
   * @return the height of the tile sprites in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] int tile_height() const noexcept { return m_tileHeight; }

  /**
   * Returns the currently selected tile IDs.
   *
   * @return the currently selected tile IDs.
   * @since 0.1.0
   */
  [[nodiscard]] const auto& selection() { return m_selection; }

 private:
  Shared<QImage> m_sheet; // FIXME this field might be unnecessary

  int m_tileWidth;
  int m_tileHeight;

  int m_nTiles;
  int m_rows;
  int m_cols;

  TileID m_firstID;

  std::set<TileID> m_selection;
};

static_assert(std::is_final_v<TileSheet>);

static_assert(std::is_nothrow_move_constructible_v<TileSheet>);
static_assert(std::is_nothrow_copy_constructible_v<TileSheet>);

}  // namespace tactile
