#include "layer_widget_context_menu.hpp"

#include "icons.hpp"

namespace tactile {

AddLayerContextMenu::AddLayerContextMenu(QWidget* parent)
    : QMenu{parent}
    , mTileLayer{addAction(IconTileLayer(), tr("Tile layer"))}
    , mObjectLayer{addAction(IconObjectLayer(), tr("Object layer"))}
{
  // clang-format off
  connect(mTileLayer, &QAction::triggered,
          this, &AddLayerContextMenu::S_AddTileLayer);
  connect(mObjectLayer, &QAction::triggered,
          this, &AddLayerContextMenu::S_AddObjectLayer);
  // clang-format on
}

LayerWidgetContextMenu::LayerWidgetContextMenu(AddLayerContextMenu* menu,
                                               QWidget* parent)
    : QMenu{parent}
    , mAdd{addMenu(IconAdd(), tr("Add new layer..."))}
{
  for (auto* action : menu->actions())
  {
    mAdd->addAction(action);
  }
}

}  // namespace tactile
