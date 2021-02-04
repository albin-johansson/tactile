#pragma once

#include <QLabel>   // QLabel
#include <QPoint>   // QPoint
#include <QWidget>  // QWidget

#include "forward_declare.hpp"
#include "tileset.hpp"

TACTILE_FORWARD_DECLARE(tactile::gui, tileset_rubber_band)

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
  QLayout* m_layout{};
  QLabel* m_imageLabel{};
  tileset_rubber_band* m_rubberBand{};
};

}  // namespace tactile::gui
