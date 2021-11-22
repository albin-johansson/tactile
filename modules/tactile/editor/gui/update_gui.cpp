#include "update_gui.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "editor/gui/layout/dock_space.hpp"
#include "editor/gui/widgets/dialogs/map_import_error_dialog.hpp"
#include "editor/gui/widgets/dialogs/resize_map_dialog.hpp"
#include "editor/gui/widgets/dialogs/save_as_dialog.hpp"
#include "editor/gui/widgets/layers/layer_dock.hpp"
#include "editor/gui/widgets/log/log_dock.hpp"
#include "editor/gui/widgets/menus/menu_bar.hpp"
#include "editor/gui/widgets/properties/properties_dock.hpp"
#include "editor/gui/widgets/tilesets/tileset_dock.hpp"
#include "editor/gui/widgets/toolbar/toolbar.hpp"
#include "editor/gui/widgets/viewport/map_view.hpp"
#include "editor/gui/widgets/viewport/viewport_widget.hpp"
#include "editor/model.hpp"
#include "icons.hpp"

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
    UpdateMapViewObjectContextMenu(*registry, dispatcher);

    UpdateLayerDock(*registry, dispatcher);
    UpdatePropertiesDock(*registry, dispatcher);
    UpdateTilesetDock(*registry, dispatcher);
    UpdateLogDock();
  }

  UpdateMapImportErrorDialog();
  UpdateResizeMapDialog(dispatcher);
  UpdateSaveAsDialog(dispatcher);
}

}  // namespace Tactile
