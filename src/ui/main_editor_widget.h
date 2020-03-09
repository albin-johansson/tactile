#pragma once
#include <QWidget>

namespace Ui {

class MainEditorWidgetUI;

}

namespace tactile {

class EditorPane;

/**
 * The <code>MainEditorWidget</code> class represents the widget that
 * contains the mouse tool bar, main tile map pane, tile sheet pane, etc. The
 * primary use for this class is to act as a container in order to be able to
 * use a stack widget to switch between the startup view and the main editor
 * view.
 *
 * @since 0.1.0
 */
class MainEditorWidget final : public QWidget {
  Q_OBJECT
 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   * @since 0.1.0
   */
  explicit MainEditorWidget(QWidget* parent = nullptr);

  ~MainEditorWidget() noexcept override;
  
  /**
   * Returns a pointer to the internal editor pane.
   *
   * @return a pointer to the internal editor pane.
   * @since 0.1.0
   */
  [[nodiscard]] EditorPane* editor() noexcept
  {
    return m_editorPane;
  }

 private:
  Ui::MainEditorWidgetUI* m_ui;
  EditorPane* m_editorPane;
};

}  // namespace tactile
