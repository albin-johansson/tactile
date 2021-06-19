#include "map_editor_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

MapEditorContextMenu::MapEditorContextMenu(QWidget* parent)
    : QMenu{parent}
    , mShowProperties{addAction(IconProperties(), tr("Show properties..."))}
{
  setObjectName(QStringLiteral(u"MapEditorContextMenu"));

  // clang-format off
  connect(mShowProperties, &QAction::triggered, this, &MapEditorContextMenu::S_ShowMapProperties);
  // clang-format on
}

}  // namespace tactile
