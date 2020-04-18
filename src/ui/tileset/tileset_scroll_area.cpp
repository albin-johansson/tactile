#include "tileset_scroll_area.h"

#include <QMouseEvent>
#include <QPainter>

#include "tileset_image_widget.h"

namespace tactile {

TilesetScrollArea::TilesetScrollArea(QWidget* parent) : QScrollArea{parent}
{}

TilesetScrollArea::~TilesetScrollArea() noexcept = default;

void TilesetScrollArea::mousePressEvent(QMouseEvent* event)
{
  QScrollArea::mousePressEvent(event);
  emit s_clicked_tile_sheet_cell(event->x(), event->y());
}

void TilesetScrollArea::mouseMoveEvent(QMouseEvent* event)
{
  QScrollArea::mouseMoveEvent(event);
  emit s_updated_tile_sheet_cell_selection(event->x(), event->y());
}

}  // namespace tactile
