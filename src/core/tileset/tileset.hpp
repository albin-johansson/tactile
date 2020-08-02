#pragma once

#include <set>

#include "smart_pointers.hpp"
#include "tile_id.hpp"
#include "type_utils.hpp"

class QImage;

namespace tactile {

/**
 * The <code>Tileset</code> class represents an image that contains a set
 * of tiles that are used to build tilemaps. All tilesets store their
 * first and last valid tile ID. tilesets should be created from images
 * that store their sprites aligned in a grid. Tiles in a tileset don't
 * need to be square.
 *
 * @since 0.1.0
 */
class Tileset final {
 public:
  /**
   * Creates a tileset with the initial first ID set to 1. Both the
   * supplied width and height will be adjusted to be at least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   * @throws BadArg if the supplied image is null.
   * @since 0.1.0
   */
  Tileset(const Shared<QImage>& image, int tileWidth, int tileHeight);

  Tileset(const Tileset& other) noexcept;

  Tileset(Tileset&& other) noexcept;

  Tileset& operator=(const Tileset& other) noexcept;

  Tileset& operator=(Tileset&& other) noexcept;

  ~Tileset() noexcept;

  /**
   * Creates and returns a unique pointer to a tileset instance. Both the
   * supplied width and height will be adjusted to be at least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   * @throws invalid_argument if the supplied image is null.
   * @return a unique pointer to a tileset.
   * @since 0.1.0
   */
  [[nodiscard]] static Unique<Tileset> unique(const Shared<QImage>& image,
                                              int tileWidth,
                                              int tileHeight);

  /**
   * Creates and returns a shared pointer to a tileset instance. Both the
   * supplied width and height will be adjusted to be at least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   * @throws invalid_argument if the supplied image is null.
   * @return a shared pointer to a tileset.
   * @since 0.1.0
   */
  [[nodiscard]] static Shared<Tileset> shared(const Shared<QImage>& image,
                                              int tileWidth,
                                              int tileHeight);

  /**
   * Sets the first tile ID property of the tileset. Note that this method
   * doesn't check if the supplied tile ID isn't already taken by another
   * tileset. This method has no effect if the supplied ID isn't greater than
   * zero.
   *
   * @param firstID the new first tile ID, must be greater than zero.
   * @since 0.1.0
   */
  void set_first_id(tile_id firstID) noexcept;

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
   * Indicates whether or not the tileset contains the specified tile ID.
   *
   * @param id the tile ID that will be checked.
   * @return true if the tileset contains the tile ID; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool contains(tile_id id) const noexcept;

  /**
   * Returns the tile ID of the tile at the specified coordinates.
   *
   * @param x the x-coordinate to check.
   * @param y the y-coordinate to check.
   * @return the tile ID of the tile at the specified position;
   * <code>empty</code> if no tile was found.
   * @since 0.1.0
   */
  [[nodiscard]] tile_id tile_at(int x, int y) const noexcept;

  /**
   * Returns the width of the tileset image.
   *
   * @return the width of the tileset image.
   * @since 0.1.0
   */
  [[nodiscard]] int width() const noexcept;

  /**
   * Returns the height of the tileset image.
   *
   * @return the height of the tileset image.
   * @since 0.1.0
   */
  [[nodiscard]] int height() const noexcept;

  /**
   * Returns the total number of rows of tiles in the tileset.
   *
   * @return the total number of rows in the tileset.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept { return m_rows; }

  /**
   * Returns the total number of columns of tiles in the tileset.
   *
   * @return the total number of columns in the tileset.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept { return m_cols; }

  /**
   * Returns the total number of tiles contained in the tileset.
   *
   * @return the total number of tiles contained in the tileset.
   * @since 0.1.0
   */
  [[nodiscard]] int tiles() const noexcept { return m_nTiles; }

  /**
   * Returns the tile ID of the first tile in the tileset. The default
   * first tile ID is 1.
   *
   * @return the tile ID of the first tile in the tileset.
   * @since 0.1.0
   */
  [[nodiscard]] tile_id first_id() const noexcept { return m_firstID; }

  /**
   * Returns the last valid tile ID associated with the tileset.
   *
   * @return the last valid tile ID associated with the tileset.
   * @since 0.1.0
   */
  [[nodiscard]] tile_id last_id() const noexcept
  {
    return first_id() + tiles();
  }

  /**
   * Returns the width of the tile sprites in the tileset. Will always be
   * at least 1.
   *
   * @return the width of the tile sprites in the tileset.
   * @since 0.1.0
   */
  [[nodiscard]] int tile_width() const noexcept { return m_tileWidth; }

  /**
   * Returns the height of the tile sprites in the tileset. Will always be
   * at least 1.
   *
   * @return the height of the tile sprites in the tileset.
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
  Shared<QImage> m_sheet;  // FIXME this field might be unnecessary

  int m_tileWidth;
  int m_tileHeight;

  int m_nTiles;
  int m_rows;
  int m_cols;

  tile_id m_firstID;

  std::set<tile_id> m_selection;
};

static_assert(validate<Tileset>());

static_assert(std::is_nothrow_move_constructible_v<Tileset>);
static_assert(std::is_nothrow_copy_constructible_v<Tileset>);

}  // namespace tactile
