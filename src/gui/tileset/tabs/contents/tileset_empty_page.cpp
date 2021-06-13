#include "tileset_empty_page.hpp"

#include "init_ui.hpp"
#include "ui_tileset_empty_page.h"

namespace tactile {

TilesetEmptyPage::TilesetEmptyPage(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::TilesetEmptyPage>(this)}
{
  // clang-format off
  connect(mUi->addSheetButton, &QPushButton::clicked, this, &TilesetEmptyPage::S_AddTileset);
  // clang-format on
}

TilesetEmptyPage::~TilesetEmptyPage() noexcept = default;

}  // namespace tactile
