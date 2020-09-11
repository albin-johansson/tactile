#pragma once

#include <qevent.h>
#include <qobject.h>

#include <map>
#include <memory>
#include <optional>

#include "core_fwd.hpp"
#include "map.hpp"
#include "map_model.hpp"
#include "tileset_model.hpp"
#include "tool_id.hpp"
#include "tool_model.hpp"
#include "types.hpp"

class QPainter;

namespace tactile::core {

/**
 * @class model
 *
 * @brief Represents the main interface for the core of the application.
 *
 * @since 0.1.0
 *
 * @headerfile model.hpp
 */
class model final : public QObject
{
  Q_OBJECT

 public:
  model();

  void resize_map(int nRows, int nCols);

  [[nodiscard]] auto add_map() -> map_id;

  /**
   * @brief Adds a tileset based on the supplied image.
   *
   * @note This method has no effect if the tileset cannot be added.
   *
   * @param image the image that contains the tile images.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @return the ID of the tileset that was added; `std::nullopt` if no tile
   * sheet was added.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto add_tileset(const QImage& image,
                                 tile_width tileWidth,
                                 tile_height tileHeight)
      -> std::optional<tileset_id>;

  void update_tileset_selection(position topLeft, position bottomRight);

  /**
   * @brief Indicates whether or not there is an active map.
   *
   * @return `true` if there is an active map; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_active_map() const noexcept -> bool;

  /**
   * @brief Returns the amount of rows in the active map.
   *
   * @return the amount of rows in the active map; `std::nullopt` if there
   * is no active map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto rows() const -> std::optional<int>;

  /**
   * @brief Returns the amount of columns in the active map.
   *
   * @return the amount of columns in the active map; `std::nullopt` if
   * there is no active map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto cols() const -> std::optional<int>;

  /**
   * @brief Returns the current width of the active map.
   *
   * @return the current width of the active map; `std::nullopt` if there is
   * no active map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto map_width() const -> std::optional<int>;

  /**
   * @brief Returns the current height of the active map.
   *
   * @return the current height of the active map; `std::nullopt` if there
   * is no active map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto map_height() const -> std::optional<int>;

  /**
   * @brief Returns the size of the tiles in the currently active map.
   *
   * @return the size of the tiles in the currently active map;
   * `std::nullopt` if there is no active map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto tile_size() const -> std::optional<int>;

  /**
   * @brief Returns a pointer to the map associated with the specified ID.
   *
   * @param id the identifier associated with the desired map.
   *
   * @return a pointer to a map; null if no map was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_map(map_id id) -> map*;

  [[nodiscard]] auto current_map_model() -> map_model*;

  [[nodiscard]] auto current_map_model() const -> const map_model*;

  [[nodiscard]] auto current_raw_map() -> map*;

  [[nodiscard]] auto current_raw_map() const -> const map*;

  [[nodiscard]] auto current_tileset() const -> const tileset*;

  [[nodiscard]] auto get_tileset_model() -> tileset_model*
  {
    return m_tilesets.get();
  }

 signals:
  void redraw();

  void disable_stamp_preview();

  void enable_stamp_preview(const position& position);

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
   * @brief Adds a row to the currently active map.
   *
   * @since 0.1.0
   */
  void add_row();

  /**
   * @brief Adds a column to the currently active map.
   *
   * @since 0.1.0
   */
  void add_col();

  /**
   * @brief Removes a row from the currently active map.
   *
   * @since 0.1.0
   */
  void remove_row();

  /**
   * @brief Removes a column from the currently active map.
   *
   * @since 0.1.0
   */
  void remove_col();

  /// @}

  void select_tool(tool_id tool);

  void remove_tileset(tileset_id id);

  void select_tileset(tileset_id id);

  /**
   * @brief Selects the tile layer associated with the specified index.
   *
   * @note This method has no effect if the supplied index isn't associated
   * with a tile layer.
   *
   * @param id the index of the tile layer that will be selected.
   *
   * @since 0.1.0
   */
  void select_layer(layer_id id);

  /**
   * @brief Selects the map associated with the specified id.
   *
   * @param id the key of the map that will be selected.
   *
   * @since 0.1.0
   */
  void select_map(map_id id);

  /**
   * @brief Closes the map associated with the specified ID.
   *
   * @param id the ID of the map that will be closed.
   *
   * @since 0.1.0
   */
  void handle_close_map(map_id id);  // FIXME not a slot

  /**
   * @brief Increases the tile size that is being used by the currently active
   * map.
   *
   * @since 0.1.0
   */
  void handle_increase_tile_size();  // FIXME not a slot

  /**
   * @brief Decreases the tile size that is being used by the currently active
   * map.
   *
   * @since 0.1.0
   */
  void handle_decrease_tile_size();  // FIXME not a slot

  /**
   * @brief Resets the tile size that is being used by the currently active
   * map to its default value.
   *
   * @since 0.1.0
   */
  void handle_reset_tile_size();  // FIXME not a slot

  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);

  void mouse_moved(QMouseEvent* event, QPointF mapPosition);

  void mouse_released(QMouseEvent* event, QPointF mapPosition);

  void mouse_entered(QEvent* event);

  void mouse_exited(QEvent* event);

 private:
  std::optional<map_id> m_currentMapID;
  std::map<map_id, map_model*> m_maps;
  std::unique_ptr<tileset_model> m_tilesets;
  tool_model m_tools;
  map_id m_nextMapID{1};

  void send_undo_redo_update();
};

}  // namespace tactile::core