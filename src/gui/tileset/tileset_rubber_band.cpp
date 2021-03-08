#include "tileset_rubber_band.hpp"

#include <algorithm>  // max

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
  emit finished_selection(get_selection());
}

void tileset_rubber_band::set_tile_width(const tile_width tileWidth) noexcept
{
  m_tileWidth = tileWidth;
}

void tileset_rubber_band::set_tile_height(const tile_height tileHeight) noexcept
{
  m_tileHeight = tileHeight;
}

void tileset_rubber_band::fit_geometry()
{
  setGeometry(adjusted_geometry());
}

auto tileset_rubber_band::adjusted_geometry() const -> QRect
{
  const auto& current = geometry();
  const auto tw = m_tileWidth.get();
  const auto th = m_tileHeight.get();

  QRect adjusted;
  adjusted.setX(current.x() - (current.x() % tw));
  adjusted.setY(current.y() - (current.y() % th));

  // Subtracts 1 at the end to avoid size that is 1 pixel larger than tile size
  adjusted.setRight(current.right() - (current.right() % tw) - 1);
  adjusted.setBottom(current.bottom() - (current.bottom() % th) - 1);

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

auto tileset_rubber_band::get_selection() const -> core::tileset_selection
{
  const auto& geo = geometry();
  const auto tw = m_tileWidth.get();
  const auto th = m_tileHeight.get();

  const core::position tl{row_t{geo.y() / th}, col_t{geo.x() / tw}};
  const core::position br{std::max(row_t{geo.bottom() / th}, tl.row()),
                          std::max(col_t{geo.right() / tw}, tl.col())};
  return {tl, br};
}

}  // namespace tactile::gui
