#include "update_gui.hpp"

#include "core/model.hpp"
#include "layout/dock_space.hpp"
#include "show_layers.hpp"
#include "show_map_viewport.hpp"
#include "show_menu_bar.hpp"
#include "show_properties.hpp"
#include "show_tilesets.hpp"
#include "widgets/toolbar/tool_bar.hpp"

namespace tactile {

void UpdateGui(const Model& model, entt::dispatcher& dispatcher)
{
  ShowMenuBar(model, dispatcher);

  UpdateDockSpace();

  Toolbar(model, dispatcher);
  ShowMapViewport(model);
  ShowLayers();
  ShowProperties();
  ShowTilesets();
}

}  // namespace tactile
