#pragma once

#include <QAction>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QWidget>
#include <memory>

namespace Ui {

class MainWindow;

}

class QPainter;
class QActionGroup;

namespace tactile {

class CentralEditorWidget;
class MouseToolWidget;
class TilesetWidget;
struct TilesetInfo;

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
   * Creates and returns a unique pointer to a tactile window.
   *
   * @param parent the parent widget, defaults to null.
   * @return a unique pointer to a tactile window.
   * @since 0.1.0
   */
  [[nodiscard]] static auto unique(QWidget* parent = nullptr)
      -> std::unique_ptr<TactileWindow>;

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

 signals:
  /**
   * A signal that is emitted when the editor pane should be redrawn.
   *
   * @since 0.1.0
   */
  void s_redraw(QPainter& painter);

  void s_new_map(int id);

  void s_close_map(int id);

  /**
   * A signal that is emitted when the user wants to add a row to the tilemap.
   *
   * @since 0.1.0
   */
  void s_added_row();

  /**
   * A signal that is emitted when the user wants to add a column to the tile
   * map.
   *
   * @since 0.1.0
   */
  void s_added_col();

  /**
   * A signal that is emitted when the user wants to remove a row from the tile
   * map.
   *
   * @since 0.1.0
   */
  void s_removed_row();

  /**
   * A signal that is emitted when the user wants to remove a column from the
   * tilemap.
   *
   * @since 0.1.0
   */
  void s_removed_col();

  /**
   * A signal that is emitted when the user wants to center the camera over the
   * tilemap.
   *
   * @since 0.1.0
   */
  void s_center_camera();

  /**
   * A signal that is emitted when the user wants to add a tileset.
   *
   * @since 0.1.0
   */
  void s_new_tileset();

  void s_resize_map();

  void s_increase_tile_size();

  void s_decrease_tile_size();

  void s_reset_tile_size();

  void s_pan_right();

  void s_pan_down();

  void s_pan_left();

  void s_pan_up();

  void s_select_map(int id);

 public slots:
  void add_tileset(const TilesetInfo& info, const QString& tabName) noexcept;

  void remove_tileset(int id) noexcept;

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
   * Centers the camera over the tilemap.
   *
   * @param mapWidth the current width of the tilemap.
   * @param mapHeight the current height of the tilemap.
   * @since 0.1.0
   */
  void center_camera(int mapWidth, int mapHeight);

  /**
   * Triggers a s_redraw of the editor pane.
   *
   * @since 0.1.0
   */
  void trigger_redraw();

  void move_camera(int dx, int dy);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private:
  Ui::MainWindow* m_ui;

  CentralEditorWidget* m_centralWidget;
  MouseToolWidget* m_mouseToolWidget;
  TilesetWidget* m_tilesetWidget;

  std::unique_ptr<QDockWidget> m_mouseToolDock;
  std::unique_ptr<QDockWidget> m_tilesetDock;

  std::unique_ptr<QActionGroup> m_mouseToolGroup;

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

  template <typename Functor>
  void on_triggered(QAction* action, Functor fun) noexcept
  {
    connect(action, &QAction::triggered, this, fun);
  }
};

}  // namespace tactile
