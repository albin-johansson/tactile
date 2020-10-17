#pragma once

#include <QActionGroup>
#include <QMainWindow>
#include <QString>
#include <QWidget>

#include "layer_dock.hpp"
#include "map_document.hpp"
#include "map_editor.hpp"
#include "position.hpp"
#include "tileset_dock.hpp"
#include "tool_dock.hpp"
#include "tool_id.hpp"
#include "types.hpp"

namespace Ui {

class window;

}

namespace tactile::gui {

/**
 * @class window
 *
 * @brief Represents the main window used in the application.
 *
 * @see `QMainWindow`
 *
 * @since 0.1.0
 *
 * @headerfile window.hpp
 */
class window final : public QMainWindow
{
  Q_OBJECT

 public:
  friend class window_connections;

  /**
   * @param parent a pointer to the parent widget, defaults to null.
   *
   * @since 0.1.0
   */
  explicit window(QWidget* parent = nullptr);

  ~window() noexcept override;

  /**
   * @brief Enables the startup view.
   *
   * @since 0.1.0
   */
  void enter_no_content_view();

  /**
   * @brief Enables the main editor view.
   *
   * @since 0.1.0
   */
  void enter_content_view();

 signals:
  void ui_save_as(const QString& path);
  void ui_open_map(const QString& path);

  void ui_new_map();
  void ui_close_map(map_id id);

  void ui_undo();
  void ui_redo();

  void ui_add_row();
  void ui_add_col();

  void ui_remove_row();
  void ui_remove_col();

  void ui_selected_tool(tool_id tool);

  void ui_requested_new_layer();
  void ui_requested_remove_layer();
  void ui_selected_layer(layer_id id);
  void ui_set_layer_visibility(bool visible);

  void ui_new_tileset();
  void ui_removed_tileset(tileset_id id);
  void ui_selected_tileset(tileset_id id);
  void ui_tileset_selection_changed(core::position topLeft,
                                    core::position bottomRight);

  void ui_resize_map();

  void ui_increase_zoom();
  void ui_decrease_zoom();
  void ui_reset_tile_size();

  void ui_pan_right();
  void ui_pan_down();
  void ui_pan_left();
  void ui_pan_up();

  void ui_select_map(map_id id);

  void mouse_pressed(QMouseEvent* event, QPointF mapPosition);
  void mouse_moved(QMouseEvent* event, QPointF mapPosition);
  void mouse_released(QMouseEvent* event, QPointF mapPosition);
  void mouse_entered(QEvent* event);
  void mouse_exited(QEvent* event);

 public slots:
  void undo_state_updated(bool canUndo);

  void redo_state_updated(bool canRedo);

  void undo_text_updated(const QString& text);

  void redo_text_updated(const QString& text);

  void added_tileset(map_id map, tileset_id id, const core::tileset& tileset);

  void removed_tileset(map_id map, tileset_id id);

  void selected_layer(layer_id id, const core::layer& layer);
  void added_layer(layer_id id, const core::layer& layer);
  void removed_layer(layer_id id);

  void switched_map(map_id map, const core::map_document& document);

  void force_redraw();

  void enable_stamp_preview(const core::position& position);

  void disable_stamp_preview();

  void handle_move_camera(int dx, int dy);

  void handle_new_map(core::map_document* map, map_id id);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  Ui::window* m_ui{};
  map_editor* m_editor{};
  tool_dock* m_toolDock{};
  layer_dock* m_layerDock{};
  tileset_dock* m_tilesetDock{};
  QActionGroup* m_toolGroup{};

  /**
   * @brief Initializes all of the connections related to the internal
   * components of the window.
   *
   * @since 0.1.0
   */
  void init_connections();

  void init_layout();

  void reset_dock_layout();

  void hide_all_docks();

  void show_all_docks();

  void center_map();

  void set_actions_enabled(bool enabled);

  /**
   * @brief Indicates whether or not the editor view is enabled.
   *
   * @return `true` if the editor view is enabled; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_editor_mode() const -> bool;

 private slots:
  void handle_remove_map(map_id tabID);

  void handle_theme_changed();

  void stamp_enabled();

  void bucket_enabled();

  void eraser_enabled();

  void handle_rectangle_enabled();

  void handle_find_same_enabled();

  void on_action_undo_triggered();

  void on_action_redo_triggered();

  void on_action_close_map_triggered();

  void on_action_tilesets_visibility_triggered();

  void on_action_mouse_tools_visibility_triggered();

  void on_action_save_triggered();

  void on_action_save_as_triggered();

  void on_action_open_map_triggered();

  void on_action_rename_triggered();

  void on_action_add_row_triggered();

  void on_action_add_column_triggered();

  void on_action_remove_row_triggered();

  void on_action_remove_column_triggered();

  void on_action_resize_map_triggered();

  void on_action_toggle_grid_triggered();

  void on_action_pan_up_triggered();

  void on_action_pan_down_triggered();

  void on_action_pan_right_triggered();

  void on_action_pan_left_triggered();

  void on_action_zoom_in_triggered();

  void on_action_zoom_out_triggered();

  void on_action_reset_zoom_triggered();

  void on_action_center_camera_triggered();

  void on_action_reset_layout_triggered();

  void on_action_stamp_tool_triggered();

  void on_action_bucket_tool_triggered();

  void on_action_eraser_tool_triggered();

  void on_action_settings_triggered();

  static void on_action_about_qt_triggered();

  static void on_action_exit_triggered();

  static void on_action_about_triggered();
};

}  // namespace tactile::gui
