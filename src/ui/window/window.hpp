#pragma once

#include <QAction>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QWidget>
#include <memory>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

namespace Ui {

class MainWindow;

}

class QPainter;
class QActionGroup;

namespace tactile::ui {

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
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   *
   * @since 0.1.0
   */
  explicit window(QWidget* parent = nullptr);

  ~window() noexcept override;

  /**
   * @copydoc window(QWidget*)
   */
  [[nodiscard]] static auto unique(QWidget* parent = nullptr)
      -> std::unique_ptr<window>;

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
  /**
   * @brief A signal that is emitted when the editor pane should be redrawn.
   *
   * @since 0.1.0
   */
  void request_redraw(QPainter& painter);

  void request_new_map(int id);

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
  void handle_redo_state_update(bool canUndo);

  void handle_add_tileset(const model::tileset_info& info,
                          const QString& tabName) noexcept;

  void handle_remove_tileset(int id) noexcept;

  /**
   * @brief Displays the about dialog.
   *
   * @since 0.1.0
   */
  void handle_display_about_dialog() noexcept;

  /**
   * @brief Displays the settings dialog.
   *
   * @since 0.1.0
   */
  void handle_display_settings_dialog() noexcept;

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

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  owner<Ui::MainWindow*> m_ui;

  central_editor_widget* m_centralWidget;
  mouse_tool_widget* m_mouseToolWidget;
  tileset_widget* m_tilesetWidget;

  std::unique_ptr<QDockWidget> m_mouseToolDock;
  std::unique_ptr<QDockWidget> m_tilesetDock;
  std::unique_ptr<QActionGroup> m_mouseToolGroup;

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
};

}  // namespace tactile::ui
