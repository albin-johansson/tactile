#pragma once

#include <QObject>
#include <map>
#include <memory>
#include <optional>

#include "core_fwd.hpp"
#include "map_model.hpp"
#include "tactile_types.hpp"
#include "tilemap.hpp"
#include "tileset_model.hpp"

class QPainter;

namespace tactile::model {

using map_id = int;

/**
 * @class core_model
 *
 * @brief Represents the main interface for the core model of the application.
 *
 * @note All tilemap mutating methods in this class have no effect if there
 * is no active tilemap.
 *
 * @since 0.1.0
 *
 * @headerfile core_model.hpp
 */
class core_model final : public QObject
{
  Q_OBJECT

 public:
  void resize_map(int nRows, int nCols);

  [[nodiscard]] auto add_map() -> int;

  /**
   * @brief Adds a tileset based on the supplied image.
   *
   * @note This method has no effect if the tileset cannot be added.
   *
   * @param fileName the image that contains the tile images.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @return the ID of the tileset that was added; `std::nullopt` if no tile
   * sheet was added.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto add_tileset(const QImage& image,
                                 int tileWidth,
                                 int tileHeight) noexcept -> std::optional<int>;

  /**
   * @brief Indicates whether or not there is an active tilemap.
   *
   * @return `true` if there is an active tilemap; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_active_map() const noexcept -> bool;

  /**
   * @brief Returns the amount of rows in the active tilemap.
   *
   * @return the amount of rows in the active tilemap; `std::nullopt` if there
   * is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the amount of columns in the active tilemap.
   *
   * @return the amount of columns in the active tilemap; `std::nullopt` if
   * there is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the current width of the active tilemap.
   *
   * @return the current width of the active tilemap; `std::nullopt` if there is
   * no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto map_width() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the current height of the active tilemap.
   *
   * @return the current height of the active tilemap; `std::nullopt` if there
   * is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto map_height() const noexcept -> std::optional<int>;

  /**
   * @brief Returns the size of the tiles in the currently active tilemap.
   *
   * @return the size of the tiles in the currently active tilemap;
   * `std::nullopt` if there is no active tilemap.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_size() const noexcept -> std::optional<int>;

  /**
   * @brief Returns a pointer to the tilemap associated with the specified ID.
   *
   * @param id the identifier associated with the desired tilemap.
   *
   * @return a pointer to a tilemap; null if no tilemap was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_map(int id) noexcept -> tilemap*;

 signals:
  void redraw_requested();

  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

  void switched_map(map_id id);

 public slots:
  /// @name Commands
  /// @{

  void undo();

  void redo();

  /**
   * @brief Adds a row to the currently active tilemap.
   *
   * @since 0.1.0
   */
  void add_row() noexcept;

  /**
   * @brief Adds a column to the currently active tilemap.
   *
   * @since 0.1.0
   */
  void add_col() noexcept;

  /**
   * @brief Removes a row from the currently active tilemap.
   *
   * @since 0.1.0
   */
  void remove_row() noexcept;

  /**
   * @brief Removes a column from the currently active tilemap.
   *
   * @since 0.1.0
   */
  void remove_col() noexcept;

  /// @}

  /**
   * @brief Selects the tile layer associated with the specified index.
   *
   * @note This method has no effect if the supplied index isn't associated
   * with a tile layer.
   *
   * @param index the index of the tile layer that will be selected.
   *
   * @since 0.1.0
   */
  void select_layer(int index) noexcept;

  /**
   * @brief Selects the tilemap associated with the specified id.
   *
   * @param id the key of the tilemap that will be selected.
   *
   * @since 0.1.0
   */
  void select_map(int id) noexcept;

  /**
   * @brief Closes the map associated with the specified ID.
   *
   * @param id the ID of the tilemap that will be closed.
   *
   * @since 0.1.0
   */
  void handle_close_map(map_id id) noexcept;  // FIXME not a slot

  /**
   * @brief Increases the tile size that is being used by the currently active
   * tilemap.
   *
   * @since 0.1.0
   */
  void handle_increase_tile_size() noexcept;  // FIXME not a slot

  /**
   * @brief Decreases the tile size that is being used by the currently active
   * tilemap.
   *
   * @since 0.1.0
   */
  void handle_decrease_tile_size() noexcept;  // FIXME not a slot

  /**
   * @brief Resets the tile size that is being used by the currently active
   * tilemap to its default value.
   *
   * @since 0.1.0
   */
  void handle_reset_tile_size() noexcept;  // FIXME not a slot

 private:
  std::optional<map_id> m_currentMapID;
  std::map<map_id, map_model*> m_maps;
  tileset_model m_tilesets;
  map_id m_nextMapID{1};

  [[nodiscard]] auto current_map() -> map_model*;
  [[nodiscard]] auto current_map() const -> const map_model*;
};

}  // namespace tactile::model