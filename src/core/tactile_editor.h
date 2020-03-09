#pragma once

#include <QObject>

#include "smart_pointers.h"

class QPainter;

namespace tactile {

class TileMap;

/**
 * The <code>TactileEditor</code> class represents the main interface for the
 * core model of the Tactile application.
 *
 * @since 0.1.0
 */
class TactileEditor final : public QObject {
  Q_OBJECT

 public:
  // TODO implement

  /**
   * Creates an instance of the Tactile editor class. There should only be one
   * instance of this class for every run of the application.
   *
   * @since 0.1.0
   */
  TactileEditor();

  ~TactileEditor() noexcept override;

  /**
   * Starts working on a new tile map. Any previous unsaved work on another tile
   * map is automatically saved before being closed.
   *
   * @since 0.1.0
   */
  void new_map() noexcept;

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
   * Adds a tile sheet based on the supplied file name. This method has no
   * effect if the supplied pointer is null.
   *
   * @param fileName the file path of the tile sheet image.
   * @since 0.1.0
   */
  void add_tile_sheet(const char* fileName) noexcept;

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
   * @return the amount of rows in the active tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int rows() const noexcept;

  /**
   * Returns the amount of columns in the active tile map.
   *
   * @return the amount of columns in the active tile map.
   * @since 0.1.0
   */
  [[nodiscard]] int cols() const noexcept;

 signals:
  /**
   * A signal that is emitted when the state of the editor has changed.
   *
   * @since 0.1.0
   */
  void updated();

 public slots:
  void draw(QPainter& painter) const noexcept;

  void add_row() noexcept;

  void add_col() noexcept;

  void remove_row() noexcept;

  void remove_col() noexcept;

 private:
  UniquePtr<TileMap> m_map;
};

}  // namespace tactile