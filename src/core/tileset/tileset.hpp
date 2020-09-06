#pragma once

#include <qimage.h>
#include <qpixmap.h>

#include <set>

#include "position.hpp"
#include "types.hpp"

namespace tactile::core {

/**
 * @class tileset
 *
 * @brief Represents a collection of tiles in a sprite sheet.
 *
 * @details A tileset is really just an image that contains a set of tiles
 * that are used to build tilemaps, where all tilesets store their first and
 * last valid tile identifiers.
 *
 * @details Tilesets must be created from images that store their sprites
 * aligned in a grid. However, the tiles don't necessarily have to be be square.
 *
 * @details The `tileset` class supports iteration of the selected cells, by
 * providing `begin` and `end` member functions.
 *
 * @since 0.1.0
 *
 * @headerfile tileset.hpp
 */
class tileset final
{
 public:
  struct selection final
  {
    position topLeft;
    position bottomRight;
  };

  using const_iterator = std::set<tile_id>::const_iterator;

  /**
   * @brief Creates a tileset with the initial first ID set to 1.
   *
   * @details Both the supplied width and height will be adjusted to be at
   * least 1.
   *
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @throws tactile_error if the supplied image is null.
   *
   * @since 0.1.0
   */
  tileset(const QImage& image, tile_width tileWidth, tile_height tileHeight);

  /**
   * @brief Creates a tileset with the initial first ID set to 1.
   *
   * @details Both the supplied width and height will be adjusted to be at
   * least 1.
   *
   * @param path the path to the image that contains the tile sprites.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @throws tactile_error if the tileset cannot be created.
   *
   * @since 0.1.0
   */
  tileset(const QString& path, tile_width tileWidth, tile_height tileHeight);

  /**
   * @brief Sets the first tile ID property of the tileset.
   *
   * @pre `firstID` must be greater than 0.
   *
   * @note This method doesn't check if the supplied tile ID isn't already
   * taken by another tileset.
   *
   * @param firstID the new first tile ID.
   *
   * @since 0.1.0
   */
  void set_first_id(tile_id firstID) noexcept;

//  /**
//   * @brief Selects the tile at the specified coordinates.
//   *
//   * @details Multiple tiles can be selected simultaneously.
//   *
//   * @note This method has no effect if the supplied position is out-of-bounds.
//   *
//   * @param x the x-coordinate of the selection.
//   * @param y the y-coordinate of the selection.
//   *
//   * @since 0.1.0
//   */
//  void select(int x, int y);

  void set_selection(position topLeft, position bottomRight);

//  /**
//   * @brief Clears any previously selected tiles.
//   *
//   * @since 0.1.0
//   */
//  void clear_selection() noexcept;

  /**
   * @brief Indicates whether or not the tileset contains the specified tile ID.
   *
   * @param id the tile ID that will be checked.
   *
   * @return `true` if the tileset contains the tile ID; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto contains(tile_id id) const noexcept -> bool;

  /**
   * @brief Returns the tile ID of the tile at the specified coordinates.
   *
   * @param x the x-coordinate to check.
   * @param y the y-coordinate to check.
   *
   * @return the tile ID of the tile at the specified position; `empty` if no
   * tile was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_at(int x, int y) const -> tile_id;

  [[nodiscard]] auto tile_at(row r, col col) const -> tile_id;

  /**
   * @brief Returns the width of the tileset image.
   *
   * @return the width of the tileset image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto width() const -> int;

  /**
   * @brief Returns the height of the tileset image.
   *
   * @return the height of the tileset image.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto height() const -> int;

  /**
   * @brief Returns the total number of rows of tiles in the tileset.
   *
   * @return the total number of rows in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const noexcept -> int;

  /**
   * @brief Returns the total number of columns of tiles in the tileset.
   *
   * @return the total number of columns in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const noexcept -> int;

  /**
   * @brief Returns the total number of tiles contained in the tileset.
   *
   * @return the total number of tiles contained in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto num_tiles() const noexcept -> int;

  /**
   * @brief Returns the tile ID of the first tile in the tileset.
   *
   * @details The default first tile ID is `1`.
   *
   * @return the ID of the first tile in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto first_id() const noexcept -> tile_id;

  /**
   * @brief Returns the last valid tile ID associated with the tileset.
   *
   * @return the last valid ID associated with the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto last_id() const noexcept -> tile_id;

  [[nodiscard]] auto image() const -> const QPixmap&;

  /**
   * @brief Returns the width of the tile sprites in the tileset.
   *
   * @note The returned value is at least 1.
   *
   * @return the width of the tile sprites in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_tile_width() const noexcept -> tile_width;

  /**
   * @brief Returns the height of the tile sprites in the tileset.
   *
   * @note The returned value is at least 1.
   *
   * @return the height of the tile sprites in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_tile_height() const noexcept -> tile_height;

  [[nodiscard]] auto get_selection() const noexcept -> const selection&;
//  /**
//   * @brief Returns a begin iterator, for iterating selected cells.
//   *
//   * @return a begin iterator.
//   *
//   * @since 0.1.0
//   */
//  [[nodiscard]] auto begin() const noexcept -> const_iterator;
//
//  /**
//   * @brief Returns an end iterator, for iterating selected cells.
//   *
//   * @return an end iterator.
//   *
//   * @since 0.1.0
//   */
//  [[nodiscard]] auto end() const noexcept -> const_iterator;

//  /**
//   * @brief Returns the current amount of selected cells.
//   *
//   * @return the amount of currently selected cells.
//   *
//   * @since 0.1.0
//   */
//  [[nodiscard]] auto num_selected() const noexcept -> int;

 private:
  QPixmap m_sheet;
  tile_id m_firstID{1};
  selection m_selection;
  int m_rows;
  int m_cols;
  tile_width m_tileWidth;
  tile_height m_tileHeight;
  int m_nTiles;
};

static_assert(std::is_final_v<tileset>);

static_assert(std::is_move_constructible_v<tileset>);
static_assert(std::is_move_assignable_v<tileset>);

static_assert(std::is_copy_constructible_v<tileset>);
static_assert(std::is_copy_assignable_v<tileset>);

}  // namespace tactile::core
