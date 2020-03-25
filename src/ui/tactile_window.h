#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QWidget>

namespace Ui {

class MainWindow;

}

class QPainter;

namespace tactile {

class CentralEditorWidget;
class MouseToolWidget;

/**
 * The <code>TactileWindow</code> class is a subclass of
 * <code>QMainWindow</code> that represents the window used in the Tactile
 * application.
 *
 * @see QMainWindow
 * @since 0.1.0
 */
class TactileWindow final : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   * @since 0.1.0
   */
  explicit TactileWindow(QWidget* parent = nullptr);

  ~TactileWindow() noexcept override;

  /**
   * Enables the startup view.
   *
   * @since 0.1.0
   */
  void enable_startup_view() noexcept;

  /**
   * Enables the main editor view.
   *
   * @since 0.1.0
   */
  void enable_editor_view() noexcept;

 public slots:
  /**
   * Displays the about dialog.
   *
   * @since 0.1.0
   */
  void display_about_dialog() noexcept;

  /**
   * Displays the settings dialog.
   *
   * @since 0.1.0
   */
  void display_settings_dialog() noexcept;

  /**
   * Centers the camera over the tile map.
   *
   * @param mapWidth the current width of the tile map.
   * @param mapHeight the current height of the tile map.
   * @since 0.1.0
   */
  void center_camera(int mapWidth, int mapHeight);

  /**
   * Triggers a redraw of the editor pane.
   *
   * @since 0.1.0
   */
  void trigger_redraw();

 signals:
  /**
   * A signal that is emitted when the editor pane should be redrawn.
   *
   * @since 0.1.0
   */
  void tw_render(QPainter& painter);

  void tw_new_map(int id);

  void tw_close_map(int id);

  /**
   * A signal that is emitted when the user wants to add a row to the tile map.
   *
   * @since 0.1.0
   */
  void tw_added_row();

  /**
   * A signal that is emitted when the user wants to add a column to the tile
   * map.
   *
   * @since 0.1.0
   */
  void tw_added_col();

  /**
   * A signal that is emitted when the user wants to remove a row from the tile
   * map.
   *
   * @since 0.1.0
   */
  void tw_removed_row();

  /**
   * A signal that is emitted when the user wants to remove a column from the
   * tile map.
   *
   * @since 0.1.0
   */
  void tw_removed_col();

  /**
   * A signal that is emitted when the user wants to center the camera over the
   * tile map.
   *
   * @since 0.1.0
   */
  void tw_center_camera();

  /**
   * A signal that is emitted when the user wants to add a tile sheet.
   *
   * @since 0.1.0
   */
  void tw_new_tile_sheet();

  void tw_resize_map();

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  Ui::MainWindow* m_ui;

  CentralEditorWidget* m_centralWidget;
  MouseToolWidget* m_mouseToolWidget;

  QDockWidget* m_mouseToolDock;

  /**
   * Initializes all of the connections related to the internal components of
   * the window.
   *
   * @since 0.1.0
   */
  void init_connections() noexcept;

  void init_layout() noexcept;

  void reset_dock_layout() noexcept;

  void hide_all_docks() noexcept;

  void show_all_docks() noexcept;

  /**
   * Indicates whether or not the editor view is enabled.
   *
   * @return true if the editor view is enabled; false otherwise.
   * @since 0.1.0
   */
  [[nodiscard]] bool in_editor_mode() const noexcept;
};

}  // namespace tactile
