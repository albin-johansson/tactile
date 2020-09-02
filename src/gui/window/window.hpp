#pragma once

#include <qaction.h>
#include <qactiongroup.h>
#include <qmainwindow.h>
#include <qpainter.h>
#include <qrect.h>
#include <qwidget.h>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"
#include "tileset_dock.hpp"
#include "tool_dock.hpp"

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
  void enable_startup_view();

  /**
   * @brief Enables the main editor view.
   *
   * @since 0.1.0
   */
  void enable_editor_view();

 signals:
  void request_new_map();

  void request_close_map(map_id id);

  void request_undo();

  void request_redo();

  void request_add_row();

  void request_add_col();

  void request_remove_row();

  void request_remove_col();

  void request_center_camera();

  void request_new_tileset();

  void request_resize_map();

  void request_increase_tile_size();

  void request_decrease_tile_size();

  void request_reset_tile_size();

  void request_pan_right();

  void request_pan_down();

  void request_pan_left();

  void request_pan_up();

  void request_select_map(map_id id);

 public slots:
  void handle_undo_state_update(bool canUndo);

  void handle_redo_state_update(bool canRedo);

  void handle_undo_text_update(const QString& text);

  void handle_redo_text_update(const QString& text);

  void handle_add_tileset(const QImage& image,
                          tileset_id id,
                          int tileWidth,
                          int tileHeight,
                          const QString& tabName);

  void handle_remove_tileset(tileset_id id);

  /**
   * @brief Centers the camera over the tilemap.
   *
   * @since 0.1.0
   */
  void handle_center_camera();

  /**
   * @brief Triggers a redraw of the editor pane.
   *
   * @since 0.1.0
   */
  void handle_draw();

  void handle_move_camera(int dx, int dy);

  void handle_new_map(not_null<model::tilemap*> map, map_id id);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  owner<Ui::window*> m_ui{};

  map_editor* m_mainEditor{};
  tool_dock* m_toolDock{};
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

  /**
   * @brief Indicates whether or not the editor view is enabled.
   *
   * @return `true` if the editor view is enabled; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_editor_mode() const -> bool;

 private slots:
  void handle_remove_tab(map_id tabID);

  void handle_theme_changed();

  void on_action_undo_triggered();

  void on_action_redo_triggered();

  void on_action_close_map_triggered();

  void on_action_tilesets_visibility_triggered();

  void on_action_mouse_tools_visibility_triggered();

  void on_action_save_triggered();

  void on_action_save_as_triggered();

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

  void on_action_rectangle_tool_triggered();

  void on_action_find_same_tool_triggered();

  void on_action_settings_triggered();

  static void on_action_about_qt_triggered();

  static void on_action_exit_triggered();

  static void on_action_about_triggered();
};

}  // namespace tactile::gui
