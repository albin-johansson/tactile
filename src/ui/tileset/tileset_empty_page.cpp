#include "tileset_empty_page.hpp"

#include "ui_tileset_empty_page.h"

namespace tactile {

TilesetEmptyPage::TilesetEmptyPage(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetEmptyPageUI{}}
{
  m_ui->setupUi(this);

  connect(m_ui->addSheetButton,
          &QPushButton::clicked,
          this,
          &TilesetEmptyPage::s_requested_tileset);
}

TilesetEmptyPage::~TilesetEmptyPage() noexcept
{
  delete m_ui;
}

}  // namespace tactile
