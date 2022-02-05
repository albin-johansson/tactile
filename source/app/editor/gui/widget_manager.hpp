#pragma once

#include <memory>  // unique_ptr
#include <string>  // string

#include <entt/entt.hpp>

#include "core/attribute_value.hpp"
#include "io/maps/parser/parse_error.hpp"
#include "tactile.hpp"

namespace tactile {

class Model;
class icon_manager;

class MenuBar;
class Toolbar;
class TilesetDock;
class LayerDock;
class PropertiesDock;
class component_dock;
class LogDock;

class ResizeMapDialog;
class MapImportErrorDialog;

class widget_manager final
{
 public:
  TACTILE_DELETE_COPY(widget_manager)
  TACTILE_DEFAULT_MOVE(widget_manager)

  widget_manager();

  ~widget_manager() noexcept;

  void update(const Model& model,
              const icon_manager& icons,
              entt::dispatcher& dispatcher);

  void show_settings();

  void show_new_map_dialog();

  void show_open_map_dialog();

  void show_add_tileset_dialog();

  void show_rename_layer_dialog(layer_id id);

  void show_add_property_dialog();

  void show_rename_property_dialog(const std::string& name);

  void show_change_property_type_dialog(std::string name, attribute_type type);

  void show_resize_map_dialog(usize currentRows, usize currentColumns);

  void show_map_import_error_dialog(parsing::parse_error error);

  void show_component_editor(const Model& model);

  void set_toolbar_visible(bool visible);

  [[nodiscard]] auto is_editor_focused() const -> bool;

  [[nodiscard]] auto is_toolbar_focused() const -> bool;

  [[nodiscard]] auto is_viewport_focused() const -> bool;

  [[nodiscard]] auto is_layer_dock_focused() const -> bool;

  [[nodiscard]] auto is_tileset_dock_focused() const -> bool;

  [[nodiscard]] auto is_properties_dock_focused() const -> bool;

  [[nodiscard]] auto is_log_dock_focused() const -> bool;

  [[nodiscard]] auto is_tileset_dock_hovered() const -> bool;

  [[nodiscard]] auto is_toolbar_visible() const -> bool;

  [[nodiscard]] auto tileset_view_width() const -> maybe<float>;

  [[nodiscard]] auto tileset_view_height() const -> maybe<float>;

 private:
  std::unique_ptr<MenuBar> mMenuBar;
  std::unique_ptr<Toolbar> mToolbar;
  std::unique_ptr<TilesetDock> mTilesetDock;
  std::unique_ptr<LayerDock> mLayerDock;
  std::unique_ptr<PropertiesDock> mPropertiesDock;
  std::unique_ptr<component_dock> mComponentDock;
  std::unique_ptr<LogDock> mLogDock;

  std::unique_ptr<ResizeMapDialog> mResizeMapDialog;
  std::unique_ptr<MapImportErrorDialog> mMapImportErrorDialog;
};

}  // namespace tactile