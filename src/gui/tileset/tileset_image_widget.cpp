#include "tileset_image_widget.hpp"

#include <QGridLayout>
#include <QMouseEvent>
#include <QRubberBand>
#include <algorithm>  // max

#include "tileset_image_label.hpp"

namespace tactile::gui {

tileset_image_widget::tileset_image_widget(const core::tileset& tileset,
                                           QWidget* parent)
    : QWidget{parent},
      m_tileWidth{tileset.get_tile_width()},
      m_tileHeight{tileset.get_tile_height()}
{
  m_layout = new QGridLayout{this};
  m_layout->setContentsMargins(0, 0, 0, 0);

  m_imageLabel = new tileset_image_label{tileset, this};
  m_rubberBand = new QRubberBand{QRubberBand::Rectangle, m_imageLabel};

  m_layout->addWidget(m_imageLabel);
  setLayout(m_layout);
}

tileset_image_widget::~tileset_image_widget() noexcept = default;

auto tileset_image_widget::get_adjusted_selection() const -> QRect
{
  const auto& rect = m_rubberBand->geometry();

  const auto tileWidth = m_tileWidth.get();
  const auto tileHeight = m_tileHeight.get();

  QRect adjusted;
  adjusted.setX(rect.x() - (rect.x() % tileWidth));
  adjusted.setY(rect.y() - (rect.y() % tileHeight));

  const auto modRight = (rect.right() % tileWidth);
  const auto modBottom = (rect.bottom() % tileHeight);
  adjusted.setRight(rect.right() - modRight);
  adjusted.setBottom(rect.bottom() - modBottom);

  if ((adjusted.width() < tileWidth) || adjusted.width() == (tileWidth + 1)) {
    adjusted.setWidth(tileWidth);
  }

  if ((adjusted.height() < tileHeight) ||
      adjusted.height() == (tileHeight + 1)) {
    adjusted.setHeight(tileHeight);
  }

  if (modRight > 0 && adjusted.width() != tileWidth) {
    adjusted.setRight(adjusted.right() + tileWidth);
  }

  if (modBottom > 0 && adjusted.height() != tileHeight) {
    adjusted.setBottom(adjusted.bottom() + tileHeight);
  }

  return adjusted;
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

      const core::position topLeft{row_t{geometry.y() / m_tileHeight.get()},
                                   col_t{geometry.x() / m_tileWidth.get()}};

      const auto calc_bottom_right = [&] {
        const row_t row{geometry.bottom() / m_tileHeight.get()};
        const col_t col{geometry.right() / m_tileWidth.get()};
        const core::position bottomRight{std::max(row - 1_row, topLeft.row()),
                                         std::max(col - 1_col, topLeft.col())};
        return bottomRight;
      };

      emit ui_set_tileset_selection({topLeft, calc_bottom_right()});
    } else {
      m_rubberBand->hide();  // selection was too small, just hide it
    }
  }
}

}  // namespace tactile::gui
