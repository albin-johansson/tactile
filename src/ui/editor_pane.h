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

 protected:
  void paintEvent(QPaintEvent* event) override;

 public:
 signals:
  /**
   * A signal method that is emitted every time that the editor pane receives
   * a paint event.
   *
   * @since 0.1.0
   */
  void received_paint_event();
};

}  // namespace tactile