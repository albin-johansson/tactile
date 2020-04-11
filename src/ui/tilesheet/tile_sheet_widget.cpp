#include "tile_sheet_widget.h"

#include "tile_sheet_content_page.h"
#include "tile_sheet_empty_page.h"
#include "ui_tile_sheet_widget.h"

namespace tactile {

TileSheetWidget::TileSheetWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TileSheetWidgetUI{}}

{
  m_ui->setupUi(this);

  m_contentPage = new TileSheetContentPage{};
  m_emptyIndex = m_ui->stackedWidget->addWidget(new TileSheetEmptyPage{});
  m_contentIndex = m_ui->stackedWidget->addWidget(m_contentPage);

  m_ui->stackedWidget->setCurrentIndex(m_emptyIndex);
}

TileSheetWidget::~TileSheetWidget() noexcept
{
  delete m_ui;
}

void TileSheetWidget::add_tile_sheet(int id, Shared<QImage> image) noexcept
{
  if (image->isNull()) {
    return;
  }

  const auto wasEmpty = m_contentPage->empty();

  m_contentPage->add_tile_sheet(id, image);

  if (wasEmpty) {
    m_ui->stackedWidget->setCurrentIndex(m_contentIndex);
  }
}

void TileSheetWidget::remove_tile_sheet(int id) noexcept
{
  m_contentPage->remove_tile_sheet(id);
}

}  // namespace tactile
