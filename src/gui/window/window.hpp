#pragma once

#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include <QPainter>
#include <QRect>
#include <QWidget>

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
 * @headerfile tactile_window.hpp
 */
class window final : public QMainWindow {
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
  void enable_startup_view() noexcept;

  /**
   * @brief Enables the main editor view.
   *
   * @since 0.1.0
   */
  void enable_editor_view() noexcept;

 signals:
  void reload_stylesheet();

  void request_redraw(QPainter& painter, const QRectF& exposed);

  void request_new_map();

  void request_close_map(int id);

  void request_undo();

  void request_redo();

  /**
   * @brief A signal that is emitted when the user wants to add a row to the
   * tilemap.
   *
   * @since 0.1.0
   */
  void request_add_row();

  /**
   * @brief A signal that is emitted when the user wants to add a column to
   * the tile map.
   *
   * @since 0.1.0
   */
  void request_add_col();

  /**
   * @brief A signal that is emitted when the user wants to remove a row from
   * the tile map.
   *
   * @since 0.1.0
   */
  void request_remove_row();

  /**
   * @brief A signal that is emitted when the user wants to remove a column
   * from the tilemap.
   *
   * @since 0.1.0
   */
  void request_remove_col();

  /**
   * @brief A signal that is emitted when the user wants to center the camera
   * over the tilemap.
   *
   * @since 0.1.0
   */
  void request_center_camera();

  /**
   * @brief A signal that is emitted when the user wants to add a tileset.
   *
   * @since 0.1.0
   */
  void request_new_tileset();

  void request_resize_map();

  void request_increase_tile_size();

  void request_decrease_tile_size();

  void request_reset_tile_size();

  void request_pan_right();

  void request_pan_down();

  void request_pan_left();

  void request_pan_up();

  void request_select_map(int id);

 public slots:
  void handle_undo_state_update(bool canUndo);

  void handle_redo_state_update(bool canRedo);

  void handle_undo_text_update(const QString& text);

  void handle_redo_text_update(const QString& text);

  void handle_add_tileset(const model::tileset_info& info,
                          const QString& tabName) noexcept;

  void handle_remove_tileset(int id) noexcept;

  /**
   * @brief Centers the camera over the tilemap.
   *
   * @param mapWidth the current width of the tilemap.
   * @param mapHeight the current height of the tilemap.
   *
   * @since 0.1.0
   */
  void handle_center_camera(int mapWidth, int mapHeight);

  /**
   * @brief Triggers a redraw of the editor pane.
   *
   * @since 0.1.0
   */
  void handle_draw();

  void handle_move_camera(int dx, int dy);

  void handle_new_map(model::core_model* core, int id);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  owner<Ui::window*> m_ui{};

  main_editor* m_centralWidget{};  // TODO rename to main_editor
  tool_dock* m_toolDock{};
  tileset_dock* m_tilesetDock{};

  QActionGroup* m_toolGroup{};

  /**
   * @brief Initializes all of the connections related to the internal
   * components of the window.
   *
   * @since 0.1.0
   */
  void init_connections() noexcept;

  void init_layout() noexcept;

  void reset_dock_layout() noexcept;

  void hide_all_docks() noexcept;

  void show_all_docks() noexcept;

  /**
   * @brief Indicates whether or not the editor view is enabled.
   *
   * @return `true` if the editor view is enabled; `false` otherwise.
   *
   * @since 0.1.0
   */
  [[nodiscard]] auto in_editor_mode() const noexcept -> bool;

  template <typename Functor>
  void on_triggered(QAction* action, Functor fun) noexcept
  {
    connect(action, &QAction::triggered, this, fun);
  }

 private slots:
  void handle_remove_tab(int tabID);

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

  static void on_action_exit_triggered();

  void on_action_settings_triggered();

  static void on_action_about_triggered();
};

}  // namespace tactile::gui
