#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QWidget>

namespace Ui {

class MainWindow;

}

class QPainter;

namespace tactile {

class EditorPane;
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
   * Clears the editor pane and emits a render signal.
   *
   * @since 0.1.0
   */
  void redraw();

  /**
   * Centers the camera over the tile map.
   *
   * @param mapWidth the current width of the tile map.
   * @param mapHeight the current height of the tile map.
   * @since 0.1.0
   */
  void center_camera(int mapWidth, int mapHeight);

  void trigger_redraw();

 signals:
  /**
   * A signal that is emitted when the editor pane should be redrawn.
   *
   * @since 0.1.0
   */
  void req_render(QPainter& painter);

  /**
   * A signal that is emitted when the user wants to add a row to the tile map.
   *
   * @since 0.1.0
   */
  void req_add_row();

  /**
   * A signal that is emitted when the user wants to add a column to the tile
   * map.
   *
   * @since 0.1.0
   */
  void req_add_col();

  /**
   * A signal that is emitted when the user wants to remove a row from the tile
   * map.
   *
   * @since 0.1.0
   */
  void req_remove_row();

  /**
   * A signal that is emitted when the user wants to remove a column from the
   * tile map.
   *
   * @since 0.1.0
   */
  void req_remove_col();

  /**
   * A signal that is emitted when the user wants to center the camera over the
   * tile map.
   *
   * @since 0.1.0
   */
  void req_center_camera();

 protected:
  void paintEvent(QPaintEvent* event) override;

 private slots:
  /**
   * Exits the Tactile application.
   *
   * @since 0.1.0
   */
  void exit() noexcept;

 private:
  Ui::MainWindow* m_ui;

  EditorPane* m_editorPane;
  MouseToolWidget* m_mouseToolWidget;

  /**
   * Initializes all of the connections related to the internal components of
   * the window.
   *
   * @since 0.1.0
   */
  void init_connections() noexcept;
};

}  // namespace tactile
