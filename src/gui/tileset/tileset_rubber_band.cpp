#include "tileset_rubber_band.hpp"

#include <QDebug>

namespace tactile::gui {

tileset_rubber_band::tileset_rubber_band(QWidget* parent)
    : QRubberBand{QRubberBand::Rectangle, parent}
{}

void tileset_rubber_band::mouse_pressed(const QPoint& pos)
{
  m_origin = pos;

  m_originRow = row_t{m_origin.y() / m_tileHeight.get()};
  m_originCol = col_t{m_origin.x() / m_tileWidth.get()};

  m_lastMousePos = m_origin;

  setGeometry(m_origin.x(),
              m_origin.y(),
              m_tileWidth.get(),
              m_tileHeight.get());

  fit_geometry();
  show();
}

void tileset_rubber_band::mouse_moved(const QPoint& pos)
{
  m_lastMousePos = pos;
  setGeometry(QRect{m_origin, pos}.normalized());
  fit_geometry();
}

void tileset_rubber_band::mouse_released()
{
  const auto& geo = geometry();
  Q_ASSERT(geo.width() >= m_tileWidth.get());
  Q_ASSERT(geo.height() >= m_tileHeight.get());
  emit finished_selection(get_selection());
}

void tileset_rubber_band::set_tile_width(tile_width tileWidth) noexcept
{
  m_tileWidth = tileWidth;
}

void tileset_rubber_band::set_tile_height(tile_height tileHeight) noexcept
{
  m_tileHeight = tileHeight;
}

void tileset_rubber_band::fit_geometry()
{
  setGeometry(adjusted_geometry());
}

auto tileset_rubber_band::adjusted_geometry() const -> QRect
{
  const auto& rect = geometry();

  const auto tw = m_tileWidth.get();
  const auto th = m_tileHeight.get();

  QRect adjusted;
  adjusted.setX(rect.x() - (rect.x() % tw));
  adjusted.setY(rect.y() - (rect.y() % th));

  const auto modRight = rect.right() % tw;
  const auto right = rect.right() - modRight;

  const auto modBottom = rect.bottom() % th;
  const auto bottom = rect.bottom() - modBottom;

  adjusted.setRight(right - 1);
  adjusted.setBottom(bottom - 1);

  adjusted.setWidth(std::max(tw, adjusted.width()));
  adjusted.setHeight(std::max(th, adjusted.height()));

  if (const row_t currentRow{m_lastMousePos.y() / th};
      currentRow != m_originRow) {
    adjusted.setHeight(adjusted.height() + th);
  }

  if (const col_t currentCol{m_lastMousePos.x() / tw};
      currentCol != m_originCol) {
    adjusted.setWidth(adjusted.width() + tw);
  }

  return adjusted;
}

auto tileset_rubber_band::get_selection() const -> core::tileset::selection
{
  const auto& geo = geometry();

  const auto tw = m_tileWidth.get();
  const auto th = m_tileHeight.get();

  const core::position topLeft{row_t{geo.y() / th}, col_t{geo.x() / tw}};
  const core::position bottomRight{
      std::max(row_t{geo.bottom() / th}, topLeft.row()),
      std::max(col_t{geo.right() / tw}, topLeft.col())};

  return {topLeft, bottomRight};
}

}  // namespace tactile::gui
