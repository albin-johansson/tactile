#pragma once

#include <QObject>
#include <unordered_map>

#include "maybe.h"
#include "smart_pointers.h"

class QPainter;

namespace tactile {

class TileMap;
class TileSheetManager;

/**
 * The <code>TactileEditor</code> class represents the main interface for the
 * core model of the Tactile application.
 *
 * <p> All tile map mutating methods of the <code>TactileEditor</code> class
 * have no effect if there is no active tile map.
 *
 * @since 0.1.0
 */
class TactileEditor final : public QObject {
  Q_OBJECT

 public:
  /**
   * Creates an instance of the Tactile editor class. There should only be one
   * instance of this class for every run of the application.
   *
   * @since 0.1.0
   */
  TactileEditor();

  ~TactileEditor() noexcept override;

  Q_DISABLE_COPY(TactileEditor)

  /**
   * Opens a tile map from the specified location.
   *
   * @param fileName the file path of the file that holds the tile map, may not
   * be null.
   * @since 0.1.0
   */
  void open_map(const char* fileName);  // TODO support TMX

  void save_as(const char* fileName) const;

  /**
   * Adds a tile sheet based on the supplied image. This method has no effect
   * if the tile sheet cannot be added.
   *
   * @param fileName the image that contains the tile images.
   * @param tileWidth the width of the tiles in the tile sheet.
   * @param tileHeight the height of the tiles in the tile sheet.
   * @return the ID of the tile sheet that was added; nothing if no tile
   * sheet was added.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<int> add_tile_sheet(const SharedPtr<QImage>& image,
                                          int tileWidth,
                                          int tileHeight) noexcept;

  /**
   * Sets the number of rows in the tile map. The amount of rows will always
   * be at least 1.
   *
   * @param nRows the new amount of rows in the tile map.
   * @since 0.1.0
   */
  void set_rows(int nRows) noexcept;

  /**
   * Sets the number of columns in the tile map. The amount of columns will
   * always be at least 1.
   *
   * @param nCols the new amount of columns in the tile map.
   * @since 0.1.0
   */
  void set_cols(int nCols) noexcept;

  /**
   * Selects the tile layer associated with the specified index. This method
   * has no effect if the supplied index isn't associated with a tile layer.
   *
   * @param index the index of the tile layer that will be selected.
   * @since 0.1.0
   */
  void select_layer(int index) noexcept;

  /**
   * Returns the amount of rows in the active tile map.
   *
   * @return the amount of rows in the active tile map; 0 if there is no
   * active tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept;

  /**
   * Returns the amount of columns in the active tile map.
   *
   * @return the amount of columns in the active tile map; 0 if there is no
   * active tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept;

 signals:
  /**
   * A signal that is emitted when the state of the active tile map has
   * changed.
   *
   * @since 0.1.0
   */
  void te_updated();

 public slots:
  void draw(QPainter& painter) const noexcept;

  /**
   * Starts working on a new tile map. Any previous unsaved work on another tile
   * map is automatically saved before being closed.
   *
   * @param id the integer that will be associated with the tile map, must be
   * unique.
   * @since 0.1.0
   */
  void new_map(int id) noexcept;  // TODO change doc

  /**
   * Closes the map associated with the specified ID.
   *
   * @param id the ID of the tile map that will be closed.
   * @since 0.1.0
   */
  void close_map(int id) noexcept;

  void add_row() noexcept;

  void add_col() noexcept;

  void remove_row() noexcept;

  void remove_col() noexcept;

 private:
  Maybe<int> m_activeMapIndex;
  std::unordered_map<int, UniquePtr<TileMap>> m_maps;
  UniquePtr<TileSheetManager> m_sheetManager;

  /**
   * Returns a pointer to the currently active map. You should check that the
   * returned pointer isn't null before using the returned pointer.
   *
   * @return a pointer to the currently active tile map; null if there is none.
   * @since 0.1.0
   */
  [[nodiscard]] TileMap* active_map() noexcept;

  /**
   * Returns a pointer to the currently active map. You should check that the
   * returned pointer isn't null before using the returned pointer.
   *
   * @return a pointer to the currently active tile map; null if there is none.
   * @since 0.1.0
   */
  [[nodiscard]] const TileMap* active_map() const noexcept;
};

}  // namespace tactile