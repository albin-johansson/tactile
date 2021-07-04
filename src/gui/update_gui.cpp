#include "update_gui.hpp"

#include "core/model.hpp"
#include "layout/dock_space.hpp"
#include "widgets/layers/layer_dock.hpp"
#include "widgets/menus/menu_bar.hpp"
#include "widgets/properties/properties_dock.hpp"
#include "widgets/tilesets/tileset_dock.hpp"
#include "widgets/toolbar/tool_bar.hpp"
#include "widgets/viewport/viewport_widget.hpp"

namespace Tactile {

void UpdateGui(const Model& model, entt::dispatcher& dispatcher)
{
  UpdateMenuBar(model, dispatcher);
  UpdateDockSpace();

  if (model.GetActiveMapId())
  {
    UpdateToolbarWidget(model, dispatcher);
  }

  UpdateViewportWidget(model, dispatcher);
  UpdateLayerDock(model, dispatcher);
  UpdatePropertiesDock(model, dispatcher);
  UpdateTilesetDock(model, dispatcher);
}

}  // namespace Tactile
