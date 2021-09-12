#include "update_gui.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/model.hpp"
#include "layout/dock_space.hpp"
#include "widgets/dialogs/map_import_error_dialog.hpp"
#include "widgets/dialogs/resize_map_dialog.hpp"
#include "widgets/dialogs/save_as_dialog.hpp"
#include "widgets/layers/layer_dock.hpp"
#include "widgets/menus/menu_bar.hpp"
#include "widgets/properties/properties_dock.hpp"
#include "widgets/tilesets/tileset_dock.hpp"
#include "widgets/toolbar/toolbar.hpp"
#include "widgets/viewport/viewport_widget.hpp"

namespace Tactile {

void UpdateGui(const Model& model, entt::dispatcher& dispatcher)
{
  UpdateMenuBar(model, dispatcher);
  UpdateDockSpace();

  if (model.HasActiveDocument())
  {
    UpdateToolbarWidget(model, dispatcher);
  }

  UpdateViewportWidget(model, dispatcher);

  if (const auto* registry = model.GetActiveRegistry())
  {
    UpdateLayerDock(*registry, dispatcher);
    UpdatePropertiesDock(*registry, dispatcher);
    UpdateTilesetDock(*registry, dispatcher);
  }

  UpdateMapImportErrorDialog();
  UpdateResizeMapDialog(dispatcher);
  UpdateSaveAsDialog(dispatcher);
}

}  // namespace Tactile
