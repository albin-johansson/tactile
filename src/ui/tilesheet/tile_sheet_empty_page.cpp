#include "tile_sheet_empty_page.h"

#include "ui_tile_sheet_empty_page.h"

namespace tactile {

TileSheetEmptyPage::TileSheetEmptyPage(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TileSheetEmptyPageUI{}}
{
  m_ui->setupUi(this);

  connect(m_ui->addSheetButton,
          &QPushButton::clicked,
          this,
          &TileSheetEmptyPage::s_requested_tile_sheet);
}

TileSheetEmptyPage::~TileSheetEmptyPage() noexcept
{
  delete m_ui;
}

}  // namespace tactile
