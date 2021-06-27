#include "update_gui.hpp"

#include "core/model.hpp"
#include "layout/dock_space.hpp"
#include "widgets/layers/layer_widget.hpp"
#include "widgets/menus/menu_bar_widget.hpp"
#include "widgets/properties/properties_widget.hpp"
#include "widgets/tilesets/tileset_widget.hpp"
#include "widgets/toolbar/tool_bar.hpp"
#include "widgets/viewport/viewport_widget.hpp"

namespace tactile {

void UpdateGui(const Model& model, entt::dispatcher& dispatcher)
{
  UpdateMenuBarWidget(model, dispatcher);

  UpdateDockSpace();

  UpdateToolbarWidget(model, dispatcher);
  UpdateViewportWidget(model, dispatcher);
  UpdateLayerWidget(model, dispatcher);
  UpdatePropertiesWidget();
  UpdateTilesetWidget();
}

}  // namespace tactile
