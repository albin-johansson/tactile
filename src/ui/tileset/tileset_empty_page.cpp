#include "tileset_empty_page.hpp"

#include "ui_tileset_empty_page.h"

namespace tactile::ui {

tileset_empty_page::tileset_empty_page(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::TilesetEmptyPageUI{}}
{
  m_ui->setupUi(this);

  connect(m_ui->addSheetButton,
          &QPushButton::clicked,
          this,
          &tileset_empty_page::request_new_tileset);
}

tileset_empty_page::~tileset_empty_page() noexcept
{
  delete m_ui;
}

}  // namespace tactile::ui
