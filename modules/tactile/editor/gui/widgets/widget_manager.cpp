#include "widget_manager.hpp"

#include "dialogs/map_import_error_dialog.hpp"
#include "dialogs/resize_map_dialog.hpp"
#include "dialogs/save_as_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/layout/dock_space.hpp"
#include "editor/model.hpp"
#include "layers/layer_dock.hpp"
#include "log/log_dock.hpp"
#include "menus/menu_bar.hpp"
#include "properties/properties_dock.hpp"
#include "tilesets/tileset_dock.hpp"
#include "toolbar/toolbar.hpp"
#include "viewport/map_view.hpp"
#include "viewport/viewport_widget.hpp"

namespace Tactile {

void WidgetManager::Update(const Model& model,
                           const Icons& icons,
                           entt::dispatcher& dispatcher)
{
  UpdateMenuBar(model, dispatcher);
  UpdateDockSpace();

  if (model.HasActiveDocument()) {
    UpdateToolbarWidget(model, dispatcher);
  }

  UpdateViewportWidget(model, icons, dispatcher);

  if (const auto* registry = model.GetActiveRegistry()) {
    UpdateMapViewObjectContextMenu(*registry, dispatcher);

    UpdateLayerDock(*registry, icons, dispatcher);
    UpdatePropertiesDock(*registry, dispatcher);
    mTilesetDock.Update(*registry, dispatcher);
    UpdateLogDock();
  }

  UpdateMapImportErrorDialog();
  UpdateResizeMapDialog(dispatcher);
  UpdateSaveAsDialog(dispatcher);
}

auto WidgetManager::IsEditorFocused() const -> bool
{
  return IsToolbarFocused() || IsViewportFocused() || IsLayerDockFocused() ||
         IsTilesetDockFocused() || IsPropertiesDockFocused() || IsLogDockFocused();
}

auto WidgetManager::IsToolbarFocused() const -> bool
{
  return Tactile::IsToolbarFocused();
}

auto WidgetManager::IsViewportFocused() const -> bool
{
  return Tactile::IsViewportFocused();
}

auto WidgetManager::IsLayerDockFocused() const -> bool
{
  return Tactile::IsLayerDockFocused();
}

auto WidgetManager::IsTilesetDockFocused() const -> bool
{
  return mTilesetDock.IsFocused();
}

auto WidgetManager::IsPropertiesDockFocused() const -> bool
{
  return Tactile::IsPropertyDockFocused();
}

auto WidgetManager::IsLogDockFocused() const -> bool
{
  return Tactile::IsLogDockFocused();
}

auto WidgetManager::IsTilesetDockHovered() const -> bool
{
  return mTilesetDock.IsHovered();
}

auto WidgetManager::GetTilesetDock() const -> const TilesetDock&
{
  return mTilesetDock;
}

auto WidgetManager::GetTilesetViewWidth() const -> Maybe<float>
{
  return mTilesetDock.GetTilesetView().GetWidth();
}

auto WidgetManager::GetTilesetViewHeight() const -> Maybe<float>
{
  return mTilesetDock.GetTilesetView().GetHeight();
}

}  // namespace Tactile