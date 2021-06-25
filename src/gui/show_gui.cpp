#include "show_gui.hpp"

#include "core/model.hpp"
#include "gui/layout/dock_space.hpp"
#include "show_layers.hpp"
#include "show_map_viewport.hpp"
#include "show_menu_bar.hpp"
#include "show_properties.hpp"
#include "show_tilesets.hpp"
#include "show_tool_bar.hpp"

namespace tactile {

void ShowGui(const Model& model, entt::dispatcher& dispatcher)
{
  ShowMenuBar(model, dispatcher);

  UpdateDockSpace();

  ShowToolBar(model, dispatcher);
  ShowMapViewport(model);
  ShowLayers();
  ShowProperties();
  ShowTilesets();
}

}  // namespace tactile
