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

struct widget_manager::widgets final
{
  MenuBar mMenuBar;
  Toolbar mToolbar;
  TilesetDock mTilesetDock;
  LayerDock mLayerDock;
  PropertiesDock mPropertiesDock;
  component_dock mComponentDock;
  LogDock mLogDock;

  ResizeMapDialog mResizeMapDialog;
  MapImportErrorDialog mMapImportErrorDialog;
};

widget_manager::widget_manager() : mWidgets{std::make_unique<widgets>()} {}

widget_manager::~widget_manager() noexcept = default;

void widget_manager::update(const Model& model,
                            const icon_manager& icons,
                            entt::dispatcher& dispatcher)
{
  mWidgets->mMenuBar.Update(model, dispatcher);
  UpdateDockSpace();

  if (model.has_active_document()) {
    mWidgets->mToolbar.Update(model, dispatcher);
    mWidgets->mLayerDock.Update(model, icons, dispatcher);
    mWidgets->mPropertiesDock.Update(model, dispatcher);
  }

  UpdateViewportWidget(model, icons, dispatcher);

  if (const auto* registry = model.active_registry()) {
    UpdateMapViewObjectContextMenu(*registry, dispatcher);

    mWidgets->mComponentDock.update(*registry, dispatcher);
    mWidgets->mTilesetDock.Update(*registry, dispatcher);

    mWidgets->mLogDock.Update();
  }

  mWidgets->mResizeMapDialog.update(model, dispatcher);
  mWidgets->mMapImportErrorDialog.update(model, dispatcher);
}

void widget_manager::show_settings()
{
  mWidgets->mMenuBar.ShowSettings();
}

void widget_manager::show_new_map_dialog()
{
  mWidgets->mMenuBar.ShowNewMapDialog();
}

void widget_manager::show_open_map_dialog()
{
  mWidgets->mMenuBar.ShowOpenMapDialog();
}

void widget_manager::show_add_tileset_dialog()
{
  mWidgets->mMenuBar.ShowAddTilesetDialog();
}

void widget_manager::show_rename_layer_dialog(const layer_id id)
{
  mWidgets->mLayerDock.ShowRenameLayerDialog(id);
}

void widget_manager::show_add_property_dialog()
{
  mWidgets->mPropertiesDock.ShowAddPropertyDialog();
}

void widget_manager::show_rename_property_dialog(const std::string& name)
{
  mWidgets->mPropertiesDock.ShowRenamePropertyDialog(name);
}

void widget_manager::show_change_property_type_dialog(std::string name,
                                                      const attribute_type type)
{
  mWidgets->mPropertiesDock.ShowChangePropertyTypeDialog(std::move(name), type);
}

void widget_manager::show_resize_map_dialog(const usize currentRows,
                                            const usize currentColumns)
{
  mWidgets->mResizeMapDialog.Show(currentRows, currentColumns);
}

void widget_manager::show_map_import_error_dialog(parsing::parse_error error)
{
  mWidgets->mMapImportErrorDialog.Open(error);
}

void widget_manager::show_component_editor(const Model& model)
{
  mWidgets->mMenuBar.ShowComponentEditor(model);
}

void widget_manager::set_toolbar_visible(const bool visible)
{
  mWidgets->mToolbar.SetVisible(visible);
}

auto widget_manager::is_editor_focused() const -> bool
{
  return is_toolbar_focused() || is_viewport_focused() || is_layer_dock_focused() ||
         is_tileset_dock_focused() || is_properties_dock_focused() ||
         is_log_dock_focused();
}

auto widget_manager::is_toolbar_focused() const -> bool
{
  return mWidgets->mToolbar.IsFocused();
}

auto widget_manager::is_viewport_focused() const -> bool
{
  return tactile::IsViewportFocused();
}

auto widget_manager::is_layer_dock_focused() const -> bool
{
  return mWidgets->mLayerDock.IsFocused();
}

auto widget_manager::is_tileset_dock_focused() const -> bool
{
  return mWidgets->mTilesetDock.IsFocused();
}

auto widget_manager::is_properties_dock_focused() const -> bool
{
  return mWidgets->mPropertiesDock.IsFocused();
}

auto widget_manager::is_log_dock_focused() const -> bool
{
  return mWidgets->mLogDock.IsFocused();
}

auto widget_manager::is_tileset_dock_hovered() const -> bool
{
  return mWidgets->mTilesetDock.IsHovered();
}

auto widget_manager::is_toolbar_visible() const -> bool
{
  return mWidgets->mToolbar.IsVisible();
}

auto widget_manager::tileset_view_width() const -> maybe<float>
{
  return mWidgets->mTilesetDock.GetTilesetView().GetWidth();
}

auto widget_manager::tileset_view_height() const -> maybe<float>
{
  return mWidgets->mTilesetDock.GetTilesetView().GetHeight();
}

}  // namespace tactile