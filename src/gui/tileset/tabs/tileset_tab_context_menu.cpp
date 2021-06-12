#include "tileset_tab_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

TilesetTabContextMenu::TilesetTabContextMenu(QWidget* parent)
    : QMenu{parent}
    , mRename{addAction(IconRename(), tr("Rename tileset"))}
    , mSep0{addSeparator()}
    , mRemove{addAction(IconRemove(), tr("Remove tileset"))}
    , mSep1{addSeparator()}
    , mShowProperties{addAction(IconProperties(), tr("Show properties"))}
{
  // clang-format off
  connect(mRename, &QAction::triggered, [this] { emit S_Rename(mIndex); });
  connect(mRemove, &QAction::triggered, [this] { emit S_Remove(mIndex); });
  connect(mShowProperties, &QAction::triggered, [this] { emit S_ShowProperties(mIndex); });
  // clang-format on
}

void TilesetTabContextMenu::SetTabIndex(const int index)
{
  mIndex = index;
}

}  // namespace tactile
