#include "tile_sheet_content_page.h"

#include "ui_tile_sheet_content_page.h"

namespace tactile {

TileSheetContentPage::TileSheetContentPage(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TileSheetContentPageUI{}}
{
  m_ui->setupUi(this);
}

TileSheetContentPage::~TileSheetContentPage() noexcept
{
  delete m_ui;
}

}  // namespace tactile
