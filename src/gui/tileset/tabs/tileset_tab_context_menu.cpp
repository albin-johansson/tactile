#include "tileset_tab_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

TilesetTabContextMenu::TilesetTabContextMenu(QWidget* parent)
    : QMenu{parent}
    , mRename{addAction(IconRename(), tr("Rename tileset"))}
    , mSep0{addSeparator()}
    , mRemove{addAction(IconRemove(), tr("Remove tileset"))}
{
  connect(mRename, &QAction::triggered, [this] { emit S_Rename(mIndex); });
  connect(mRemove, &QAction::triggered, [this] { emit S_Remove(mIndex); });
}

void TilesetTabContextMenu::SetTabIndex(const int index)
{
  mIndex = index;
}

}  // namespace tactile
