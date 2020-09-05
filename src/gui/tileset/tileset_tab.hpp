#pragma once

#include <qlayout.h>
#include <qpoint.h>
#include <qscrollarea.h>
#include <qwidget.h>

#include "fwd.hpp"
#include "position.hpp"
#include "types.hpp"

namespace tactile::gui {

class tileset_tab final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_tab(const QImage& image,
                       tileset_id id,
                       tile_width tileWidth,
                       tile_height tileHeight,
                       QWidget* parent = nullptr);

  ~tileset_tab() noexcept override;

  [[nodiscard]] auto id() const noexcept -> tileset_id
  {
    return m_id;
  }

 signals:
  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QScrollArea* m_scrollArea;
  tileset_image_widget* m_imageWidget;
  QLayout* m_layout;
  QPoint m_lastMousePos;
  tileset_id m_id;
};

}  // namespace tactile::gui
