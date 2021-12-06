#include "widget_manager.hpp"

#include <utility>  // move

#include "dialogs/map_import_error_dialog.hpp"
#include "dialogs/resize_map_dialog.hpp"
#include "dialogs/save_as_dialog.hpp"
#include "editor/model.hpp"
#include "icons.hpp"
#include "layout/dock_space.hpp"
#include "log/log_dock.hpp"
#include "viewport/map_view.hpp"
#include "viewport/viewport_widget.hpp"

namespace Tactile {

void WidgetManager::Update(const Model& model,
                           const Icons& icons,
                           entt::dispatcher& dispatcher)
{
  mMenuBar.Update(model, dispatcher);
  UpdateDockSpace();

  if (model.HasActiveDocument()) {
    mToolbar.Update(model, dispatcher);
  }

  UpdateViewportWidget(model, icons, dispatcher);

  if (const auto* registry = model.GetActiveRegistry()) {
    UpdateMapViewObjectContextMenu(*registry, dispatcher);

    mLayerDock.Update(*registry, icons, dispatcher);
    mPropertiesDock.Update(*registry, dispatcher);
    mTilesetDock.Update(*registry, dispatcher);
    mResizeMapDialog.Update(*registry, dispatcher);

    UpdateLogDock();
  }

  UpdateMapImportErrorDialog();
  UpdateSaveAsDialog(dispatcher);
}

void WidgetManager::ShowSettings()
{
  mMenuBar.ShowSettings();
}

void WidgetManager::ShowNewMapDialog()
{
  mMenuBar.ShowNewMapDialog();
}

void WidgetManager::ShowOpenMapDialog()
{
  mMenuBar.ShowOpenMapDialog();
}

void WidgetManager::ShowAddTilesetDialog()
{
  mMenuBar.ShowAddTilesetDialog();
}

void WidgetManager::ShowRenameLayerDialog(const LayerID id)
{
  mLayerDock.ShowRenameLayerDialog(id);
}

void WidgetManager::ShowAddPropertyDialog()
{
  mPropertiesDock.ShowAddPropertyDialog();
}

void WidgetManager::ShowRenamePropertyDialog(const std::string& name)
{
  mPropertiesDock.ShowRenamePropertyDialog(name);
}

void WidgetManager::ShowChangePropertyTypeDialog(std::string name,
                                                 const PropertyType type)
{
  mPropertiesDock.ShowChangePropertyTypeDialog(std::move(name), type);
}

void WidgetManager::ShowResizeMapDialog(const usize currentRows,
                                        const usize currentColumns)
{
  mResizeMapDialog.Show(currentRows, currentColumns);
}

void WidgetManager::SetToolbarVisible(const bool visible)
{
  mToolbar.SetVisible(visible);
}

auto WidgetManager::IsEditorFocused() const -> bool
{
  return IsToolbarFocused() || IsViewportFocused() || IsLayerDockFocused() ||
         IsTilesetDockFocused() || IsPropertiesDockFocused() || IsLogDockFocused();
}

auto WidgetManager::IsToolbarFocused() const -> bool
{
  return mToolbar.IsFocused();
}

auto WidgetManager::IsViewportFocused() const -> bool
{
  return Tactile::IsViewportFocused();
}

auto WidgetManager::IsLayerDockFocused() const -> bool
{
  return mLayerDock.IsFocused();
}

auto WidgetManager::IsTilesetDockFocused() const -> bool
{
  return mTilesetDock.IsFocused();
}

auto WidgetManager::IsPropertiesDockFocused() const -> bool
{
  return mPropertiesDock.IsFocused();
}

auto WidgetManager::IsLogDockFocused() const -> bool
{
  return Tactile::IsLogDockFocused();
}

auto WidgetManager::IsTilesetDockHovered() const -> bool
{
  return mTilesetDock.IsHovered();
}

auto WidgetManager::IsToolbarVisible() const -> bool
{
  return mToolbar.IsVisible();
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