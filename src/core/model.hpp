#pragma once

#include <QObject>

#include "layer.hpp"
#include "layer_id.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "property.hpp"
#include "property_model.hpp"
#include "tileset.hpp"
#include "tileset_id.hpp"
#include "tool_id.hpp"
#include "tool_model.hpp"

namespace tactile::core {

class map_document;
class map_document_manager;
class tile_layer;

/**
 * \class model
 *
 * \brief Represents the main interface for the core of the application.
 *
 * \since 0.1.0
 *
 * \headerfile model.hpp
 */
class model final : public QObject
{
  Q_OBJECT

 public:
  model();

  /**
   * \copydoc map_document_manager::add()
   */
  [[nodiscard]] auto add_map() -> map_id;

  /**
   * \copydoc map_document_manager::add(map_document* document)
   */
  [[nodiscard]] auto add_map(map_document* document) -> map_id;

  /**
   * \copydoc map_document_manager::has_active_map()
   */
  [[nodiscard]] auto has_active_map() const noexcept -> bool;

  /**
   * \copydoc map_document_manager::at()
   */
  [[nodiscard]] auto get_document(map_id id) -> map_document*;

  /**
   * \copydoc map_document_manager::current_map_id()
   */
  [[nodiscard]] auto current_map_id() const -> maybe<map_id>;

  /**
   * \copydoc map_document_manager::current_document()
   */
  [[nodiscard]] auto current_document() -> map_document*;

  /**
   * \copydoc current_document()
   */
  [[nodiscard]] auto current_document() const -> const map_document*;

 signals:
  void redraw();

  void switched_map(map_id id,
                    const map_document& document,
                    const vm::shared_property_model& propertyModel);

  void added_layer(layer_id id, const layer& layer);
  void added_duplicated_layer(layer_id id, const layer& layer);
  void selected_layer(layer_id id, const layer& layer);
  void removed_layer(layer_id id);
  void moved_layer_back(layer_id id);
  void moved_layer_forward(layer_id id);

  void disable_stamp_preview();
  void enable_stamp_preview(const position& position);

  void undo_state_updated(bool canUndo);
  void redo_state_updated(bool canRedo);
  void undo_text_updated(const QString& text);
  void redo_text_updated(const QString& text);
  void clean_changed(bool clean);

  void added_tileset(map_id map, tileset_id id, const tileset& tileset);
  void removed_tileset(tileset_id id);

  void added_property(const QString& name, const core::property& property);
  void removed_property(const QString& name);
  void renamed_property(const QString& oldName, const QString& newName);
  void moved_property_up(const QString& name);
  void moved_property_down(const QString& name);
  void duplicated_property(const QString& name);

 public slots:
  /**
   * \brief Reverts the effects of the last performed command.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \since 0.1.0
   *
   * \signal `redraw`, `clean_changed`
   */
  void undo();

  /**
   * \brief Restores the effects of the last undone command.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \since 0.1.0
   *
   * \signal `redraw`
   */
  void redo();

  /**
   * \brief Resizes the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \param nRows the new number of rows, must be greater than 0.
   * \param nCols the new number of columns, must be greater than 0.
   *
   * \see cmd::resize_map
   *
   * \since 0.1.0
   *
   * \signal `redraw`
   */
  void resize_map(row_t nRows, col_t nCols);

  /**
   * \brief Adds a row to the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::add_row
   *
   * \since 0.1.0
   *
   * \signal `redraw`
   */
  void add_row();

  /**
   * \brief Adds a column to the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::add_col
   *
   * \since 0.1.0
   *
   * \signal `redraw`
   */
  void add_col();

  /**
   * \brief Removes a row from the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::remove_row
   *
   * \since 0.1.0
   */
  void remove_row();

  /**
   * \brief Removes a column from the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \see cmd::remove_col
   *
   * \since 0.1.0
   */
  void remove_col();

  /**
   * \brief Adds a layer to the currently active map.
   *
   * \note This function has no effect if there is no active map document.
   *
   * \since 0.1.0
   *
   * \signal `added_layer`
   */
  void add_layer();

  /**
   * \brief Removes the specified layer from the currently active map.
   *
   * \param id the ID associated with the layer that will be removed.
   *
   * \since 0.1.0
   *
   * \signal `removed_layer`.
   */
  void remove_layer(layer_id id);

  /**
   * \brief Selects the tile layer associated with the specified index.
   *
   * \note This method has no effect if the supplied index isn't associated
   * with a tile layer or if there is no active map document.
   *
   * \param id the index of the tile layer that will be selected.
   *
   * \since 0.1.0
   *
   * \signal `selected_layer`, `redraw`
   */
  void select_layer(layer_id id);

  /**
   * \copydoc tool_model::select(tool_id)
   */
  void select_tool(tool_id id);

  /**
   * \copydoc map_document::select_tileset()
   */
  void select_tileset(tileset_id id);

  /**
   * \copydoc map_document::set_tileset_selection()
   */
  void set_tileset_selection(const tileset::selection& selection);

  /**
   * \copydoc map_document::set_layer_visibility()
   * \signal `redraw`
   */
  void set_layer_visibility(layer_id id, bool visible);

  /**
   * \copydoc map_document::set_layer_opacity()
   * \signal `redraw`
   */
  void set_layer_opacity(layer_id id, double opacity);

  /**
   * \copydoc map_document::set_layer_name()
   * \signal `redraw`
   */
  void set_layer_name(layer_id id, const QString& name);

  /**
   * \copydoc map_document::move_layer_back()
   * \signal `redraw`
   */
  void move_layer_back(layer_id id);

  /**
   * \copydoc map_document::move_layer_forward()
   * \signal `redraw`
   */
  void move_layer_forward(layer_id id);

  /**
   * \copydoc map_document::duplicate_layer()
   * \signal `redraw`
   */
  void duplicate_layer(layer_id id);

  /**
   * \brief Increases the tile size that is being used by the currently active
   * map.
   *
   * \since 0.1.0
   */
  void increase_tile_size();

  /**
   * \brief Decreases the tile size that is being used by the currently active
   * map.
   *
   * \since 0.1.0
   */
  void decrease_tile_size();

  /**
   * \brief Resets the tile size that is being used by the currently active
   * map to its default value.
   *
   * \since 0.1.0
   */
  void reset_tile_size();

  // clang-format off
  /**
   * \copydoc map_document::add_tileset(const QImage&, const QFileInfo&, const QString&, tile_width, tile_height)
   */
  void create_tileset(const QImage& image,
                      const QFileInfo& path,
                      const QString& name,
                      tile_width tileWidth,
                      tile_height tileHeight);
  // clang-format on

  /**
   * \copydoc map_document::ui_remove_tileset()
   * \signal `redraw`
   */
  void remove_tileset(tileset_id id);

  /**
   * \copydoc map_document::set_tileset_name()
   */
  void set_tileset_name(tileset_id id, const QString& name);

  void add_property(const QString& name, core::property::type type);

  void remove_property(const QString& name);

  void rename_property(const QString& oldName, const QString& newName);

  void set_property(const QString& name, const core::property& property);

  /**
   * \copydoc map_document_manager::select()
   * \signal `switched_map`
   */
  void select_map(map_id id);

  /**
   * \copydoc map_document_manager::close()
   */
  void close_map(map_id id);

  /**
   * \copydoc tool_model::pressed()
   */
  void mouse_pressed(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \copydoc tool_model::moved()
   */
  void mouse_moved(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \copydoc tool_model::released()
   */
  void mouse_released(QMouseEvent* event, const QPointF& mapPosition);

  /**
   * \copydoc tool_model::entered()
   */
  void mouse_entered(QEvent* event);

  /**
   * \copydoc tool_model::exited()
   */
  void mouse_exited(QEvent* event);

 private:
  map_document_manager* m_mapDocuments{};
  tool_model m_tools;
};

}  // namespace tactile::core