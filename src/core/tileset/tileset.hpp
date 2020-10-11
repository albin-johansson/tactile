#pragma once

#include <qimage.h>
#include <qpixmap.h>
#include <qrect.h>
#include <qstring.h>

#include <concepts>       // invocable
#include <optional>       // optional
#include <unordered_map>  // unordered_map

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
  /**
   * @struct tileset::selection
   *
   * @brief Represents the selection of tiles in a tileset.
   *
   * @details The two positions represent a rectangular selection.
   *
   * @since 0.1.0
   *
   * @headerfile tileset.hpp
   */
  struct selection final
  {
    position topLeft;      ///< The top-left corner position.
    position bottomRight;  ///< The bottom-right corner position.
  };

  /**
   * @brief Creates a tileset.
   *
   * @details Both the supplied width and height will be adjusted to be at
   * least 1.
   *
   * @param firstID the first tile ID associated with the tileset.
   * @param image the image that contains the tile sprites, mustn't be null.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @throws tactile_error if the supplied image is null.
   *
   * @since 0.1.0
   */
  tileset(tile_id firstID,
          const QImage& image,
          tile_width tileWidth,
          tile_height tileHeight);

  /**
   * @brief Creates a tileset.
   *
   * @details Both the supplied width and height will be adjusted to be at
   * least 1.
   *
   * @param firstID the first tile ID associated with the tileset.
   * @param path the path to the image that contains the tile sprites.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @throws tactile_error if the tileset cannot be created.
   *
   * @since 0.1.0
   */
  tileset(tile_id firstID,
          const QString& path,
          tile_width tileWidth,
          tile_height tileHeight);

  /**
   * @brief Iterates the current selection.
   *
   * @details This function has no effect if there is no current selection.
   *
   * @tparam T the type of the callable.
   *
   * @param callable the callable that will be invoked for each tile in the
   * selection.
   *
   * @since 0.1.0
   */
  template <std::invocable<position> T>
  void iterate_selection(T&& callable) const
  {
    if (m_selection) {
      const auto& [topLeft, bottomRight] = *m_selection;

      const auto nRows = 1_row + (bottomRight.row() - topLeft.row());
      const auto nCols = 1_col + (bottomRight.col() - topLeft.col());

      for (row_t row{0}; row < nRows; ++row) {
        for (col_t col{0}; col < nCols; ++col) {
          callable({row, col});
        }
      }
    }
  }

  /**
   * @brief Iterates the currently selected tiles in the tileset.
   *
   * @details This function is for example useful for the stamp tool, which
   * needs to iterate the selected tiles in the tileset, whilst also keeping
   * track of which position in the map needs to be changed.
   *
   * @note This function has no effect if there is no current selection.
   *
   * @tparam T the type of the callable, the first parameter is the map
   * position, the second parameter is the tileset position.
   *
   * @param mapOrigin the origin tile map position.
   * @param callable the callable that will be invoked for each selected tile in
   * the tileset.
   *
   * @since 0.1.0
   */
  template <std::invocable<position, position> T>
  void iterate_selection(const position& mapOrigin, T&& callable) const
  {
    if (m_selection) {
      const auto& [topLeft, bottomRight] = *m_selection;

      if (topLeft == bottomRight) {
        callable(mapOrigin, topLeft);
      } else {
        const auto diff = bottomRight - topLeft;
        const auto endRow = 1_row + diff.row();
        const auto endCol = 1_col + diff.col();
        for (row_t row{0}; row < endRow; ++row) {
          for (col_t col{0}; col < endCol; ++col) {
            const auto tilePos = mapOrigin.offset_by(row, col);
            const auto tilesetPos = topLeft.offset_by(row, col);
            callable(tilePos, tilesetPos);
          }
        }
      }
    }
  }

  /**
   * @brief Sets the current selection in the tileset.
   *
   * @param topLeft the top-left position of the selection.
   * @param bottomRight the bottom-right position of the selection.
   *
   * @since 0.1.0
   */
  void set_selection(const position& topLeft, const position& bottomRight);

  /**
   * @brief Clears any current selection.
   *
   * @since 0.1.0
   */
  void clear_selection() noexcept;

  /**
   * @brief Sets the name of the tileset.
   *
   * @param name the new name of the tileset.
   *
   * @since 0.1.0
   */
  void set_name(QString name);

  void set_path(QString path);

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
   * @brief Indicates whether or not a single tile is selected in the tileset.
   *
   * @return `true` if only a single tile is selected; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto is_single_tile_selected() const noexcept -> bool;

  /**
   * @brief Returns the ID of the tile at the specified position.
   *
   * @param position the position of the desired tile.
   *
   * @return the ID of the tile at the specified position; `empty` if the
   * position was out-of-bounds.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_at(const position& position) const -> tile_id;

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
   * @brief Returns the image source rectangle associated with the specified
   * tile.
   *
   * @param id the ID of the tile to obtain the source rectangle of.
   *
   * @return the source rectangle associated with the tile; `std::nullopt` if
   * no source rectangle was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto image_source(tile_id id) const -> std::optional<QRect>;

  /**
   * @brief Returns the image associated with the tileset.
   *
   * @return the image associated with the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto image() const -> const QPixmap&
  {
    return m_image;
  }

  /**
   * @brief Returns the width of the tile sprites in the tileset.
   *
   * @note The returned value is at least 1.
   *
   * @return the width of the tile sprites in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_tile_width() const noexcept -> tile_width
  {
    return m_tileWidth;
  }

  /**
   * @brief Returns the height of the tile sprites in the tileset.
   *
   * @note The returned value is at least 1.
   *
   * @return the height of the tile sprites in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_tile_height() const noexcept -> tile_height
  {
    return m_tileHeight;
  }

  /**
   * @brief Returns the current selection in the tileset.
   *
   * @return the current selection in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_selection() const noexcept
      -> const std::optional<selection>&
  {
    return m_selection;
  }

  /**
   * @brief Returns the total number of rows of tiles in the tileset.
   *
   * @return the total number of rows in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const noexcept -> row_t
  {
    return m_numRows;
  }

  /**
   * @brief Returns the total number of columns of tiles in the tileset.
   *
   * @return the total number of columns in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const noexcept -> col_t
  {
    return m_numCols;
  }

  /**
   * @brief Returns the tile ID of the first tile in the tileset.
   *
   * @details The default first tile ID is `1`.
   *
   * @return the ID of the first tile in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto first_id() const noexcept -> tile_id
  {
    return m_firstID;
  }

  /**
   * @brief Returns the last valid tile ID associated with the tileset.
   *
   * @return the last valid ID associated with the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto last_id() const noexcept -> tile_id
  {
    return m_lastID;
  }

  /**
   * @brief Returns the amount of tiles in the tileset.
   *
   * @return the amount of tiles in the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_count() const noexcept -> int
  {
    return m_numTiles;
  }

  /**
   * @brief Returns the name associated with the tileset.
   *
   * @return the name of the tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto name() const -> const QString&
  {
    return m_name;
  }

  [[nodiscard]] auto path() const -> const QString&
  {
    return m_path;
  }

 private:
  QPixmap m_image;
  tile_id m_firstID{1};
  tile_id m_lastID;
  std::optional<selection> m_selection;
  std::unordered_map<tile_id, QRect> m_sourceRects;
  tile_width m_tileWidth{};
  tile_height m_tileHeight{};
  row_t m_numRows{};
  col_t m_numCols{};
  int m_numTiles{};
  QString m_path{};
  QString m_name{QStringLiteral(u"Untitled")};
};

static_assert(std::is_final_v<tileset>);

static_assert(std::is_move_constructible_v<tileset>);
static_assert(std::is_move_assignable_v<tileset>);

static_assert(std::is_copy_constructible_v<tileset>);
static_assert(std::is_copy_assignable_v<tileset>);

}  // namespace tactile::core
