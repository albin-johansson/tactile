#include "tile_sheet_widget.h"

#include "ui_tile_sheet_widget.h"

namespace tactile {

TileSheetWidget::TileSheetWidget(QWidget* parent) : QWidget{parent}
{
  m_ui->setupUi(this);
}

TileSheetWidget::~TileSheetWidget() noexcept
{
  delete m_ui;
}

}  // namespace tactile
