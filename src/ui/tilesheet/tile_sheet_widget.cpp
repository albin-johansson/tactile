#include "tile_sheet_widget.h"

#include "tile_sheet_content_page.h"
#include "tile_sheet_empty_page.h"
#include "ui_tile_sheet_widget.h"

namespace tactile {

TileSheetWidget::TileSheetWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TileSheetWidgetUI{}}

{
  m_ui->setupUi(this);

  m_emptyIndex = m_ui->stackedWidget->addWidget(new TileSheetEmptyPage{});
  m_contentIndex = m_ui->stackedWidget->addWidget(new TileSheetContentPage{});

  m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);
}

TileSheetWidget::~TileSheetWidget() noexcept
{
  delete m_ui;
}

}  // namespace tactile
