#include "update_gui.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "tactile/editor/gui/layout/dock_space.hpp"
#include "tactile/editor/gui/widgets/dialogs/map_import_error_dialog.hpp"
#include "tactile/editor/gui/widgets/dialogs/resize_map_dialog.hpp"
#include "tactile/editor/gui/widgets/dialogs/save_as_dialog.hpp"
#include "tactile/editor/gui/widgets/layers/layer_dock.hpp"
#include "tactile/editor/gui/widgets/menus/menu_bar.hpp"
#include "tactile/editor/gui/widgets/properties/properties_dock.hpp"
#include "tactile/editor/gui/widgets/tilesets/tileset_dock.hpp"
#include "tactile/editor/gui/widgets/toolbar/toolbar.hpp"
#include "tactile/editor/gui/widgets/viewport/viewport_widget.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

void UpdateGui(const Model& model, entt::dispatcher& dispatcher)
{
  UpdateMenuBar(model, dispatcher);
  UpdateDockSpace();

  if (model.HasActiveDocument()) {
    UpdateToolbarWidget(model, dispatcher);
  }

  UpdateViewportWidget(model, dispatcher);

  if (const auto* registry = model.GetActiveRegistry()) {
    UpdateLayerDock(*registry, dispatcher);
    UpdatePropertiesDock(*registry, dispatcher);
    UpdateTilesetDock(*registry, dispatcher);
  }

  UpdateMapImportErrorDialog();
  UpdateResizeMapDialog(dispatcher);
  UpdateSaveAsDialog(dispatcher);
}

}  // namespace Tactile
