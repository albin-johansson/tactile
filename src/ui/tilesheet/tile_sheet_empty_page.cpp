#include "tile_sheet_empty_page.h"

#include "ui_tile_sheet_empty_page.h"

namespace tactile {

TileSheetEmptyPage::TileSheetEmptyPage(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TileSheetEmptyPageUI{}}
{
  m_ui->setupUi(this);
}

TileSheetEmptyPage::~TileSheetEmptyPage() noexcept
{
  delete m_ui;
}

}  // namespace tactile
