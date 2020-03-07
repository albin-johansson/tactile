#pragma once

#include <QRect>
#include <QWidget>

namespace tactile {

/**
 * The <code>EditorPane</code> class represents the center state pane that
 * contains the main surface that is used to render the tile maps. This class
 * is a subclass of <code>QWidget</code>.
 *
 * @see QWidget
 * @since 0.1.0
 */
class EditorPane final : public QWidget {
  Q_OBJECT

 public:
  /**
   * @param parent a pointer to the parent widget, defaults to null.
   * @since 0.1.0
   */
  explicit EditorPane(QWidget* parent = nullptr) noexcept;

  /**
   * Moves the viewport by the specified amount.
   *
   * @param dx the offset that will be applied in the x-axis, may be negative.
   * @param dy the offset that will be applied in the y-axis, may be negative.
   * @since 0.1.0
   */
  void move_viewport(int dx, int dy) noexcept;

  /**
   * Sets the position of the viewport.
   *
   * @param x the new x-coordinate of the viewport.
   * @param y the new y-coordinate of the viewport.
   * @since 0.1.0
   */
  void set_viewport_pos(int x, int y) noexcept;

  /**
   * Returns the viewport of the editor pane.
   *
   * @return the viewport of the editor pane.
   * @since 0.1.0
   */
  [[nodiscard]] const QRect& viewport() const noexcept
  {
    return m_viewport;
  }

 signals:
  /**
   * A signal method that is emitted every time that the editor pane receives
   * a paint event and wants the tile map to be redrawn.
   *
   * @since 0.1.0
   */
  void req_redraw();

 protected:
  void paintEvent(QPaintEvent* event) override;

  void resizeEvent(QResizeEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  // TODO add support for touch pads by overriding wheelEvent stuff

 private:
  QRect m_viewport;

  int m_lastMouseX;
  int m_lastMouseY;
};

}  // namespace tactile