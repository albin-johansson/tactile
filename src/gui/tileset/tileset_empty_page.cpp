#include "tileset_empty_page.hpp"

#include "ui_tileset_empty_page.h"

namespace tactile::gui {

tileset_empty_page::tileset_empty_page(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::tileset_empty_page{}}
{
  m_ui->setupUi(this);

  connect(m_ui->addSheetButton,
          &QPushButton::clicked,
          this,
          &tileset_empty_page::ui_add_tileset);
}

tileset_empty_page::~tileset_empty_page() noexcept
{
  delete m_ui;
}

}  // namespace tactile::gui
