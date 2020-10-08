#pragma once

#include <qlabel.h>
#include <qpoint.h>
#include <qrubberband.h>
#include <qwidget.h>

#include "position.hpp"
#include "tileset.hpp"
#include "types.hpp"

namespace tactile::gui {

class tileset_image_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_image_widget(const core::tileset& tileset,
                                QWidget* parent = nullptr);

  ~tileset_image_widget() noexcept override;

 signals:
  void tileset_selection_changed(core::position topLeft,
                                 core::position bottomRight);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QLayout* m_layout;
  QLabel* m_imageLabel;
  QRubberBand* m_rubberBand;
  QPoint m_origin;
  QPoint m_lastMousePos;
  tile_width m_tileWidth;
  tile_height m_tileHeight;

  [[nodiscard]] auto get_adjusted_selection() const -> QRect;
};

}  // namespace tactile::gui
