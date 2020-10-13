#pragma once

#include <QEvent>
#include <QImage>
#include <QObject>
#include <QString>
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

  /**
   * @brief Adds a map document to the model.
   *
   * @return the map ID that was associated with the document.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto add_map() -> map_id
  {
    return m_maps->add();
  }

  /**
   * @brief Adds a map document to the model.
   *
   * @note The model claims ownership of the supplied document.
   *
   * @param document a pointer to the document that will be added.
   *
   * @return the map ID that was associated with the document.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto add_map(map_document* document) -> map_id
  {
    return m_maps->add(document);
  }

  void update_tileset_selection(position topLeft, position bottomRight)
  {
    m_maps->update_tileset_selection(topLeft, bottomRight);
  }

  void resize_map(row_t nRows, col_t nCols);

  /**
   * @copydoc map_manager::has_active_map()
   */
  [[nodiscard]] auto has_active_map() const noexcept -> bool
  {
    return m_maps->has_active_map();
  }

  /**
   * @copydoc map_manager::at()
   */
  [[nodiscard]] auto get_document(map_id id) -> map_document*
  {
    return m_maps->at(id);
  }

  /**
   * @copydoc map_manager::current_map()
   */
  [[nodiscard]] auto current_map() const -> std::optional<map_id>
  {
    return m_maps->current_map();
  }

  /**
   * @copydoc map_manager::current_document()
   */
  [[nodiscard]] auto current_document() -> map_document*
  {
    return m_maps->current_document();
  }

  /**
   * @copydoc current_document()
   */
  [[nodiscard]] auto current_document() const -> const map_document*
  {
    return m_maps->current_document();
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

  void added_tileset(map_id map, tileset_id id, const tileset& tileset);

  void removed_tileset(map_id map, tileset_id id);

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

  /**
   * @copydoc tool_model::select(tool_id)
   */
  void select_tool(tool_id tool)
  {
    m_tools.select(tool);
  }

  /**
   * @copydoc tool_model::select_tileset(tileset_id)
   */
  void select_tileset(tileset_id id)
  {
    m_maps->select_tileset(id);
  }

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

  /**
   * @brief Adds a tileset based on the supplied image.
   *
   * @note This method has no effect if the tileset cannot be added.
   *
   * @param image the image that contains the tile images.
   * @param path the file path of the tileset image.
   * @param name the name associated with the tileset.
   * @param tileWidth the width of the tiles in the tileset.
   * @param tileHeight the height of the tiles in the tileset.
   *
   * @since 0.1.0
   */
  void ui_added_tileset(const QImage& image,
                        const QString& path,
                        const QString& name,
                        tile_width tileWidth,
                        tile_height tileHeight)
  {
    m_maps->ui_added_tileset(image, path, name, tileWidth, tileHeight);
  }

  void ui_removed_tileset(tileset_id id);

  /**
   * @brief Selects the map associated with the specified id.
   *
   * @param id the key of the map that will be selected.
   *
   * @since 0.1.0
   */
  void ui_selected_map(map_id id);

  /**
   * @brief Closes the map associated with the specified ID.
   *
   * @param id the ID of the map that will be closed.
   *
   * @since 0.1.0
   */
  void close_map(map_id id)
  {
    m_maps->close(id);
  }

  /**
   * @copydoc tool_model::pressed(QMouseEvent*,QPointF)
   */
  void mouse_pressed(QMouseEvent* event, QPointF mapPosition)
  {
    m_tools.pressed(event, mapPosition);
  }

  /**
   * @copydoc tool_model::moved(QMouseEvent*,QPointF)
   */
  void mouse_moved(QMouseEvent* event, QPointF mapPosition)
  {
    m_tools.moved(event, mapPosition);
  }

  /**
   * @copydoc tool_model::released(QMouseEvent*,QPointF)
   */
  void mouse_released(QMouseEvent* event, QPointF mapPosition)
  {
    m_tools.released(event, mapPosition);
  }

  /**
   * @copydoc tool_model::entered(QMouseEvent*,QPointF)
   */
  void mouse_entered(QEvent* event)
  {
    m_tools.entered(event);
  }

  /**
   * @copydoc tool_model::exited(QMouseEvent*,QPointF)
   */
  void mouse_exited(QEvent* event)
  {
    m_tools.exited(event);
  }

 private:
  map_manager* m_maps{};
  tool_model m_tools;
};

}  // namespace tactile::core