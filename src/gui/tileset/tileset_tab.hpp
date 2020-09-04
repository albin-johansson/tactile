#pragma once

#include <qlayout.h>
#include <qpoint.h>
#include <qscrollarea.h>
#include <qwidget.h>

#include "fwd.hpp"
#include "types.hpp"

namespace tactile::gui {

class tileset_tab final : public QWidget
{
 public:
  explicit tileset_tab(const QImage& image,
                       tile_width tileWidth,
                       tile_height tileHeight,
                       QWidget* parent = nullptr);

  ~tileset_tab() noexcept override;

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QScrollArea* m_scrollArea;
  tileset_image_widget* m_imageWidget;
  QLayout* m_layout;
  QPoint m_lastMousePos;
};

}  // namespace tactile::gui
