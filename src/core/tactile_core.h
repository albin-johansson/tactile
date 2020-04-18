#pragma once

#include <QObject>
#include <unordered_map>

#include "maybe.h"
#include "smart_pointers.h"
#include "tactile_types.h"

class QPainter;

namespace tactile {

class Tilemap;
class TileSheetManager;

/**
 * The <code>TactileCore</code> class represents the main interface for the
 * core model of the Tactile application.
 *
 * <p> All tilemap mutating methods of the <code>TactileCore</code> class
 * have no effect if there is no active tilemap.
 *
 * @since 0.1.0
 */
class TactileCore final : public QObject {
  Q_OBJECT

 public:
  /**
   * Creates an instance of the Tactile editor class. There should only be one
   * instance of this class for every run of the application.
   *
   * @since 0.1.0
   */
  TactileCore();

  ~TactileCore() noexcept override;

  Q_DISABLE_COPY(TactileCore)

  /**
   * Opens a tilemap from the specified location.
   *
   * @param fileName the file path of the file that holds the tilemap, may not
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
  [[nodiscard]] Maybe<int> add_tile_sheet(const Shared<QImage>& image,
                                          int tileWidth,
                                          int tileHeight) noexcept;

  /**
   * Sets the number of rows in the tilemap. The amount of rows will always
   * be at least 1.
   *
   * @param nRows the new amount of rows in the tilemap.
   * @since 0.1.0
   */
  void set_rows(int nRows) noexcept;

  /**
   * Sets the number of columns in the tilemap. The amount of columns will
   * always be at least 1.
   *
   * @param nCols the new amount of columns in the tilemap.
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
   * Selects the tilemap associated with the specified id. This method has
   * no effect if the supplied key isn't associated with a tilemap.
   *
   * @param id the key of the tilemap that will be selected.
   * @since 0.1.0
   */
  void select_map(int id) noexcept;

  /**
   * Returns the amount of rows in the active tilemap.
   *
   * @return the amount of rows in the active tilemap; nothing if there is no
   * active tilemap.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<int> rows() const noexcept;

  /**
   * Returns the amount of columns in the active tilemap.
   *
   * @return the amount of columns in the active tilemap; nothing if there
   * is no active tilemap.
   * @since 0.1.0
   */
  [[nodiscard]] Maybe<int> cols() const noexcept;

  [[nodiscard]] Maybe<int> map_width() const noexcept;

  [[nodiscard]] Maybe<int> map_height() const noexcept;

  [[nodiscard]] Maybe<int> tile_size() const noexcept;

 signals:
  /**
   * A signal that is emitted when the state of the active tilemap has
   * changed.
   *
   * @since 0.1.0
   */
  void s_updated();

 public slots:
  void draw(QPainter& painter) const noexcept;

  /**
   * Starts working on a new tilemap. The new tilemap will be made active.
   * This method has no effect if the supplied ID is already associated with
   * a tilemap.
   *
   * @param id the integer that will be associated with the tilemap, must be
   * unique.
   * @since 0.1.0
   */
  void new_map(int id) noexcept;

  /**
   * Closes the map associated with the specified ID.
   *
   * @param id the ID of the tilemap that will be closed.
   * @since 0.1.0
   */
  void close_map(int id) noexcept;

  void add_row() noexcept;

  void add_col() noexcept;

  void remove_row() noexcept;

  void remove_col() noexcept;

  void increase_tile_size() noexcept;

  void decrease_tile_size() noexcept;

  void reset_tile_size() noexcept;

 private:
  Maybe<int> m_activeMapIndex;
  HashMap<int, Unique<Tilemap>> m_maps;
  Unique<TileSheetManager> m_sheetManager;

  /**
   * Returns a pointer to the currently active map. You should check that the
   * returned pointer isn't null before using the returned pointer.
   *
   * @return a pointer to the currently active tilemap; null if there is none.
   * @since 0.1.0
   */
  [[nodiscard]] Tilemap* active_map() noexcept;

  /**
   * Returns a pointer to the currently active map. You should check that the
   * returned pointer isn't null before using the returned pointer.
   *
   * @return a pointer to the currently active tilemap; null if there is none.
   * @since 0.1.0
   */
  [[nodiscard]] const Tilemap* active_map() const noexcept;

  /**
   * A templated helper method for queries that returns something of the
   * <code>Maybe</code> type from the active tilemap.
   *
   * @tparam ReturnType the type of the return value.
   * @tparam Functor the type of the function object.
   * @param fun the function object that performs the query, has to take
   * a reference to a <code>Tilemap</code> as a parameter.
   * @return either a value of the return type, or <code>nothing</code>.
   * @since 0.1.0
   */
  template <typename ReturnType, typename Functor>
  [[nodiscard]] Maybe<ReturnType> maybe_get(Functor fun) const
  {
    if (auto* map = active_map(); map) {
      return fun(*map);
    } else {
      return nothing;
    }
  }
};

}  // namespace tactile