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

widget_manager::widget_manager()
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

widget_manager::~widget_manager() noexcept = default;

void widget_manager::update(const Model& model,
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

void widget_manager::show_settings()
{
  mMenuBar->ShowSettings();
}

void widget_manager::show_new_map_dialog()
{
  mMenuBar->ShowNewMapDialog();
}

void widget_manager::show_open_map_dialog()
{
  mMenuBar->ShowOpenMapDialog();
}

void widget_manager::show_add_tileset_dialog()
{
  mMenuBar->ShowAddTilesetDialog();
}

void widget_manager::show_rename_layer_dialog(const layer_id id)
{
  mLayerDock->ShowRenameLayerDialog(id);
}

void widget_manager::show_add_property_dialog()
{
  mPropertiesDock->ShowAddPropertyDialog();
}

void widget_manager::show_rename_property_dialog(const std::string& name)
{
  mPropertiesDock->ShowRenamePropertyDialog(name);
}

void widget_manager::show_change_property_type_dialog(std::string name,
                                                      const attribute_type type)
{
  mPropertiesDock->ShowChangePropertyTypeDialog(std::move(name), type);
}

void widget_manager::show_resize_map_dialog(const usize currentRows,
                                            const usize currentColumns)
{
  mResizeMapDialog->Show(currentRows, currentColumns);
}

void widget_manager::show_map_import_error_dialog(parsing::parse_error error)
{
  mMapImportErrorDialog->Open(error);
}

void widget_manager::show_component_editor(const Model& model)
{
  mMenuBar->ShowComponentEditor(model);
}

void widget_manager::set_toolbar_visible(const bool visible)
{
  mToolbar->SetVisible(visible);
}

auto widget_manager::is_editor_focused() const -> bool
{
  return is_toolbar_focused() || is_viewport_focused() || is_layer_dock_focused() ||
         is_tileset_dock_focused() || is_properties_dock_focused() ||
         is_log_dock_focused();
}

auto widget_manager::is_toolbar_focused() const -> bool
{
  return mToolbar->IsFocused();
}

auto widget_manager::is_viewport_focused() const -> bool
{
  return tactile::IsViewportFocused();
}

auto widget_manager::is_layer_dock_focused() const -> bool
{
  return mLayerDock->IsFocused();
}

auto widget_manager::is_tileset_dock_focused() const -> bool
{
  return mTilesetDock->IsFocused();
}

auto widget_manager::is_properties_dock_focused() const -> bool
{
  return mPropertiesDock->IsFocused();
}

auto widget_manager::is_log_dock_focused() const -> bool
{
  return mLogDock->IsFocused();
}

auto widget_manager::is_tileset_dock_hovered() const -> bool
{
  return mTilesetDock->IsHovered();
}

auto widget_manager::is_toolbar_visible() const -> bool
{
  return mToolbar->IsVisible();
}

auto widget_manager::tileset_view_width() const -> maybe<float>
{
  return mTilesetDock->GetTilesetView().GetWidth();
}

auto widget_manager::tileset_view_height() const -> maybe<float>
{
  return mTilesetDock->GetTilesetView().GetHeight();
}

}  // namespace tactile