#include "tileset_image_widget.hpp"

#include <QEvent>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPainter>
#include <algorithm>  // max

#include "tileset_image_label.hpp"

using namespace tactile::core;

namespace tactile::gui {

tileset_image_widget::tileset_image_widget(const tileset& tileset,
                                           QWidget* parent)
    : QWidget{parent},
      m_tileWidth{tileset.get_tile_width()},
      m_tileHeight{tileset.get_tile_height()}
{
  m_layout = new QGridLayout{this};
  m_layout->setMargin(0);

  m_imageLabel = new tileset_image_label{tileset, this};
  m_rubberBand = new QRubberBand{QRubberBand::Rectangle, m_imageLabel};

  m_layout->addWidget(m_imageLabel);
  setLayout(m_layout);
}

tileset_image_widget::~tileset_image_widget() noexcept = default;

auto tileset_image_widget::get_adjusted_selection() const -> QRect
{
  const auto& geometry = m_rubberBand->geometry();

  const auto gX = geometry.x();
  const auto gY = geometry.y();
  const auto gRight = geometry.right();
  const auto gBottom = geometry.bottom();

  const auto tileWidth = m_tileWidth.get();
  const auto tileHeight = m_tileHeight.get();

  QRect newGeometry;
  newGeometry.setX(gX - (gX % tileWidth));
  newGeometry.setY(gY - (gY % tileHeight));

  const auto modRight = gRight % tileWidth;
  newGeometry.setRight(gRight - modRight);

  const auto modBottom = gBottom % tileHeight;
  newGeometry.setBottom(gBottom - modBottom);

  if (const auto width = newGeometry.width();
      (width < tileWidth) || width == (tileWidth + 1)) {
    newGeometry.setWidth(tileWidth);
  }

  if (const auto height = newGeometry.height();
      (height < tileHeight) || height == (tileHeight + 1)) {
    newGeometry.setHeight(tileHeight);
  }

  return newGeometry;
}

void tileset_image_widget::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  m_lastMousePos = event->pos();

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    m_origin = m_lastMousePos;

    const QPoint bottomRight{m_origin.x() + m_tileWidth.get(),
                             m_origin.y() + m_tileHeight.get()};

    m_rubberBand->setGeometry(QRect{m_origin, bottomRight});
    m_rubberBand->setGeometry(get_adjusted_selection());
    m_rubberBand->show();
  }
}

void tileset_image_widget::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);

  // TODO improve the accuracy of the selection when selecting multiple tiles

  const auto pos = event->pos();

  if (event->buttons() & Qt::MouseButton::LeftButton) {
    m_rubberBand->setGeometry(QRect{m_origin, pos}.normalized());
    m_rubberBand->setGeometry(get_adjusted_selection());
  }

  m_lastMousePos = pos;
}

void tileset_image_widget::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);

  if (event->button() == Qt::MouseButton::LeftButton) {
    const auto selection = get_adjusted_selection();

    if (selection.width() >= m_tileWidth.get() &&
        selection.height() >= m_tileHeight.get()) {
      m_rubberBand->setGeometry(selection);

      const auto& geometry = m_rubberBand->geometry();

      const position topLeft{row_t{geometry.y() / m_tileHeight.get()},
                             col_t{geometry.x() / m_tileWidth.get()}};

      const auto calc_bottom_right = [&] {
        const row_t row{geometry.bottom() / m_tileHeight.get()};
        const col_t col{geometry.right() / m_tileWidth.get()};
        const position bottomRight{std::max(row - 1_row, topLeft.row()),
                                   std::max(col - 1_col, topLeft.col())};
        return bottomRight;
      };

      emit tileset_selection_changed(topLeft, calc_bottom_right());
    } else {
      m_rubberBand->hide();  // selection was too small, just hide it
    }
  }
}

}  // namespace tactile::gui
