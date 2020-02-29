#pragma once

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QWidget>

namespace Ui {
class MainWindow;
}

namespace tactile {

class EditorPane;

/**
 * The <code>TactileWindow</code> class is a subclass of
 * <code>QOpenGLWindow</code> that represents the window used in the Tactile
 * application.
 *
 * @see QOpenGLWindow
 * @since 0.1.0
 */
class TactileWindow final : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @since 0.1.0
   */
  explicit TactileWindow(QWidget* parent = nullptr);

  ~TactileWindow() override;

 private:
  Ui::MainWindow* m_ui;
  EditorPane* m_editorPane;
};

}  // namespace tactile
