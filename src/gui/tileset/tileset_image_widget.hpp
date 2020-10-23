#pragma once

#include <QPoint>
#include <QWidget>

#include "position.hpp"
#include "tile_height.hpp"
#include "tile_width.hpp"
#include "tileset.hpp"

class QLabel;
class QRubberBand;

namespace tactile::gui {

class tileset_image_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit tileset_image_widget(const core::tileset& tileset,
                                QWidget* parent = nullptr);

  ~tileset_image_widget() noexcept override;

 signals:
  void ui_set_tileset_selection(const core::tileset::selection& selection);

 protected:
  void mousePressEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

 private:
  tile_width m_tileWidth;
  tile_height m_tileHeight;
  QLayout* m_layout;
  QLabel* m_imageLabel;
  QRubberBand* m_rubberBand;
  QPoint m_origin;
  QPoint m_lastMousePos;

  [[nodiscard]] auto get_adjusted_selection() const -> QRect;
};

}  // namespace tactile::gui
