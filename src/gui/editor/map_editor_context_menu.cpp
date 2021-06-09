#include "map_editor_context_menu.hpp"

namespace tactile {

MapEditorContextMenu::MapEditorContextMenu(QWidget* parent)
    : QMenu{parent}
    , mShowProperties{addAction(tr("Show properties..."))}
{
  // clang-format off
  connect(mShowProperties, &QAction::triggered, this, &MapEditorContextMenu::S_ShowMapProperties);
  // clang-format on
}

}  // namespace tactile
