#pragma once

#include "smart_pointers.h"
#include "tile_id.h"

class QImage;

namespace tactile {

/**
 * The <code>TileSheet</code> class represents an image that contains a set
 * of tiles that are used to build tile maps. All tile sheets store their
 * first and last valid tile ID.
 *
 * @since 0.1.0
 */
class TileSheet final {
 public:
  /**
   * Creates a tile sheet with the initial first ID set to 1.
   *
   * @param image the image that contains the tile sprites, cannot be null.
   * @param size the size (width and height) of all of the tiles in the tile
   * sheet image. Must be greater than zero. Will be adjusted to be at least 1.
   * @throws invalid_argument if the supplied image is null.
   * @since 0.1.0
   */
  TileSheet(const SharedPtr<QImage>& image, int size);

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
   * Indicates whether or not the tile sheet contains the specified tile ID.
   *
   * @param id the tile ID that will be checked.
   * @return true if the tile sheet contains the tile ID; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool contains(TileID id) const noexcept;

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
   * Returns the total number of tiles contained in the tile sheet.
   *
   * @return the total number of tiles contained in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] int tiles() const noexcept
  {
    return m_nTiles;
  }

  /**
   * Returns the tile ID of the first tile in the tile sheet. The default
   * first tile ID is 1.
   *
   * @return the tile ID of the first tile in the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] TileID first_id() const noexcept
  {
    return m_firstID;
  }

  /**
   * Returns the last valid tile ID associated with the tile sheet.
   *
   * @return the last valid tile ID associated with the tile sheet.
   * @since 0.1.0
   */
  [[nodiscard]] TileID last_id() const noexcept
  {
    return first_id() + tiles();
  }

 private:
  SharedPtr<QImage> m_sheet;
  int m_nTiles;
  TileID m_firstID;
};

}  // namespace tactile
