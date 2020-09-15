#pragma once

#include <qevent.h>
#include <qobject.h>

#include <memory>    // unique_ptr
#include <optional>  // optional

#include "core_fwd.hpp"
#include "map.hpp"
#include "map_document.hpp"
#include "map_manager.hpp"
#include "tileset_manager.hpp"
#include "tool_id.hpp"
#include "tool_model.hpp"
#include "types.hpp"
#include "vector_map.hpp"

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

  void resize_map(row_t nRows, col_t nCols);

  /**
   * @brief Indicates whether or not there is an active map.
   *
   * @return `true` if there is an active map; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto has_active_map() const noexcept -> bool
  {
    return m_maps->has_active_map();
  }

  /**
   * @brief Returns a pointer to the map associated with the specified ID.
   *
   * @param id the identifier associated with the desired map.
   *
   * @return a pointer to a map; null if no map was found.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_map(map_id id) -> map*
  {
    return m_maps->at(id);
  }

  /**
   * @brief Returns a pointer to the current map document.
   *
   * @return a pointer to the current map document; `nullptr` if there is no
   * active map document.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto current_map_document() -> map_document*
  {
    return m_maps->current_document();
  }

  /**
   * @copydoc current_map_document()
   */
  [[nodiscard]] auto current_map_document() const -> const map_document*
  {
    return m_maps->current_document();
  }

  /**
   * @brief Returns a pointer to the current map.
   *
   * @return a pointer to the current map; `nullptr` if there is no active map.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto current_map() -> map*
  {
    return m_maps->current_map();
  }

  /**
   * @copydoc current_document()
   */
  [[nodiscard]] auto current_map() const -> const map*
  {
    return m_maps->current_map();
  }

  /**
   * @brief Returns a pointer to the currently active tileset.
   *
   * @return a pointer to the currently active tileset; `nullptr` if there is
   * no active tileset.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto current_tileset() const -> const tileset*
  {
    return m_tilesets->current_tileset();
  }

  /**
   * @brief Returns a pointer to the tileset manager.
   *
   * @return a pointer to the tileset manager, won't be null.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto get_tileset_manager() -> tileset_manager*
  {
    return m_tilesets.get();
  }

 signals:
  void redraw();

  void switched_map(map_id id);

  void disable_stamp_preview();

  void enable_stamp_preview(const position& position);

  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

 public slots:
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

  void select_tool(tool_id tool);

  void select_tileset(tileset_id id);

  /**
   * @brief Increases the tile size that is being used by the currently active
   * map.
   *
   * @since 0.1.0
   */
  void increase_tile_size();

  /**
   * @brief Decreases the tile size that is being used by the currently active
   * map.
   *
   * @since 0.1.0
   */
  void decrease_tile_size();

  /**
   * @brief Resets the tile size that is being used by the currently active
   * map to its default value.
   *
   * @since 0.1.0
   */
  void reset_tile_size();

  void remove_tileset(tileset_id id);

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
  void close_map(map_id id);

  void mouse_pressed(QMouseEvent* event, QPointF mapPosition)
  {
    m_tools.pressed(event, mapPosition);
  }

  void mouse_moved(QMouseEvent* event, QPointF mapPosition)
  {
    m_tools.moved(event, mapPosition);
  }

  void mouse_released(QMouseEvent* event, QPointF mapPosition)
  {
    m_tools.released(event, mapPosition);
  }

  void mouse_entered(QEvent* event)
  {
    m_tools.entered(event);
  }

  void mouse_exited(QEvent* event)
  {
    m_tools.exited(event);
  }

 private:
  std::unique_ptr<map_manager> m_maps;
  std::unique_ptr<tileset_manager> m_tilesets;
  tool_model m_tools;
};

}  // namespace tactile::core