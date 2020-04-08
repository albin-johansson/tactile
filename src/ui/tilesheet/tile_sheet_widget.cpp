#include "tile_sheet_widget.h"

#include "tile_sheet_empty_page.h"
#include "ui_tile_sheet_widget.h"
#include "widget_size_policy.h"

namespace tactile {

TileSheetWidget::TileSheetWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TileSheetWidgetUI{}}

{
  m_ui->setupUi(this);

  m_emptySheetIndex = m_ui->stackedWidget->addWidget(new TileSheetEmptyPage{});
  m_ui->stackedWidget->setCurrentIndex(m_emptySheetIndex);
}

TileSheetWidget::~TileSheetWidget() noexcept
{
  delete m_ui;
}

}  // namespace tactile
