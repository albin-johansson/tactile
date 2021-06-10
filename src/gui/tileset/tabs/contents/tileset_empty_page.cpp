#include "tileset_empty_page.hpp"

#include "init_ui.hpp"
#include "ui_tileset_empty_page.h"

namespace tactile {

TilesetEmptyPage::TilesetEmptyPage(QWidget* parent)
    : QWidget{parent}
    , mUi{init_ui<Ui::TilesetEmptyPage>(this)}
{
  connect(mUi->addSheetButton,
          &QPushButton::clicked,
          this,
          &TilesetEmptyPage::S_AddTileset);
}

TilesetEmptyPage::~TilesetEmptyPage() noexcept = default;

}  // namespace tactile
