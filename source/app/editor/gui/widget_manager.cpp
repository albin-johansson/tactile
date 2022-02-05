#include "widget_manager.hpp"

#include <utility>  // move

#include "components/component_dock.hpp"
#include "dialogs/map_import_error_dialog.hpp"
#include "dialogs/resize_map_dialog.hpp"
#include "editor/model.hpp"
#include "icons.hpp"
#include "layers/layer_dock.hpp"
#include "layout/dock_space.hpp"
#include "log/log_dock.hpp"
#include "menus/menu_bar.hpp"
#include "properties/properties_dock.hpp"
#include "tilesets/tileset_dock.hpp"
#include "toolbar/toolbar.hpp"
#include "viewport/map_view.hpp"
#include "viewport/viewport_widget.hpp"

namespace tactile {

WidgetManager::WidgetManager()
    : mMenuBar{std::make_unique<MenuBar>()}
    , mToolbar{std::make_unique<Toolbar>()}
    , mTilesetDock{std::make_unique<TilesetDock>()}
    , mLayerDock{std::make_unique<LayerDock>()}
    , mPropertiesDock{std::make_unique<PropertiesDock>()}
    , mComponentDock{std::make_unique<ComponentDock>()}
    , mLogDock{std::make_unique<LogDock>()}
    , mResizeMapDialog{std::make_unique<ResizeMapDialog>()}
    , mMapImportErrorDialog{std::make_unique<MapImportErrorDialog>()}
{}

WidgetManager::~WidgetManager() noexcept = default;

void WidgetManager::Update(const Model& model,
                           const icon_manager& icons,
                           entt::dispatcher& dispatcher)
{
  mMenuBar->Update(model, dispatcher);
  UpdateDockSpace();

  if (model.HasActiveDocument()) {
    mToolbar->Update(model, dispatcher);
    mLayerDock->Update(model, icons, dispatcher);
    mPropertiesDock->Update(model, dispatcher);
  }

  UpdateViewportWidget(model, icons, dispatcher);

  if (const auto* registry = model.GetActiveRegistry()) {
    UpdateMapViewObjectContextMenu(*registry, dispatcher);

    mComponentDock->Update(*registry, dispatcher);
    mTilesetDock->Update(*registry, dispatcher);

    mLogDock->Update();
  }

  mResizeMapDialog->Update(model, dispatcher);
  mMapImportErrorDialog->Update(model, dispatcher);
}

void WidgetManager::ShowSettings()
{
  mMenuBar->ShowSettings();
}

void WidgetManager::ShowNewMapDialog()
{
  mMenuBar->ShowNewMapDialog();
}

void WidgetManager::ShowOpenMapDialog()
{
  mMenuBar->ShowOpenMapDialog();
}

void WidgetManager::ShowAddTilesetDialog()
{
  mMenuBar->ShowAddTilesetDialog();
}

void WidgetManager::ShowRenameLayerDialog(const layer_id id)
{
  mLayerDock->ShowRenameLayerDialog(id);
}

void WidgetManager::ShowAddPropertyDialog()
{
  mPropertiesDock->ShowAddPropertyDialog();
}

void WidgetManager::ShowRenamePropertyDialog(const std::string& name)
{
  mPropertiesDock->ShowRenamePropertyDialog(name);
}

void WidgetManager::ShowChangePropertyTypeDialog(std::string name,
                                                 const attribute_type type)
{
  mPropertiesDock->ShowChangePropertyTypeDialog(std::move(name), type);
}

void WidgetManager::ShowResizeMapDialog(const usize currentRows,
                                        const usize currentColumns)
{
  mResizeMapDialog->Show(currentRows, currentColumns);
}

void WidgetManager::ShowMapImportErrorDialog(const parsing::parse_error error)
{
  mMapImportErrorDialog->Open(error);
}

void WidgetManager::ShowComponentEditor(const Model& model)
{
  mMenuBar->ShowComponentEditor(model);
}

void WidgetManager::SetToolbarVisible(const bool visible)
{
  mToolbar->SetVisible(visible);
}

auto WidgetManager::IsEditorFocused() const -> bool
{
  return IsToolbarFocused() || IsViewportFocused() || IsLayerDockFocused() ||
         IsTilesetDockFocused() || IsPropertiesDockFocused() || IsLogDockFocused();
}

auto WidgetManager::IsToolbarFocused() const -> bool
{
  return mToolbar->IsFocused();
}

auto WidgetManager::IsViewportFocused() const -> bool
{
  return tactile::IsViewportFocused();
}

auto WidgetManager::IsLayerDockFocused() const -> bool
{
  return mLayerDock->IsFocused();
}

auto WidgetManager::IsTilesetDockFocused() const -> bool
{
  return mTilesetDock->IsFocused();
}

auto WidgetManager::IsPropertiesDockFocused() const -> bool
{
  return mPropertiesDock->IsFocused();
}

auto WidgetManager::IsLogDockFocused() const -> bool
{
  return mLogDock->IsFocused();
}

auto WidgetManager::IsTilesetDockHovered() const -> bool
{
  return mTilesetDock->IsHovered();
}

auto WidgetManager::IsToolbarVisible() const -> bool
{
  return mToolbar->IsVisible();
}

auto WidgetManager::GetTilesetViewWidth() const -> maybe<float>
{
  return mTilesetDock->GetTilesetView().GetWidth();
}

auto WidgetManager::GetTilesetViewHeight() const -> maybe<float>
{
  return mTilesetDock->GetTilesetView().GetHeight();
}

}  // namespace tactile