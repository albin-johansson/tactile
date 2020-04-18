#include "tile_sheet_scroll_area.h"

#include <QMouseEvent>
#include <QPainter>

#include "tile_sheet_image_widget.h"

namespace tactile {

TileSheetScrollArea::TileSheetScrollArea(QWidget* parent) : QScrollArea{parent}
{}

TileSheetScrollArea::~TileSheetScrollArea() noexcept = default;

void TileSheetScrollArea::mousePressEvent(QMouseEvent* event)
{
  QScrollArea::mousePressEvent(event);
  emit s_clicked_tile_sheet_cell(event->x(), event->y());
}

void TileSheetScrollArea::mouseMoveEvent(QMouseEvent* event)
{
  QScrollArea::mouseMoveEvent(event);
  emit s_updated_tile_sheet_cell_selection(event->x(), event->y());
}

}  // namespace tactile
