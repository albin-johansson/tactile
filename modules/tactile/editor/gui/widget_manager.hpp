#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "dialogs/resize_map_dialog.hpp"
#include "layers/layer_dock.hpp"
#include "menus/menu_bar.hpp"
#include "properties/properties_dock.hpp"
#include "tilesets/tileset_dock.hpp"
#include "toolbar/toolbar.hpp"

namespace Tactile {

class Model;
class Icons;

class WidgetManager final {
 public:
  void Update(const Model& model, const Icons& icons, entt::dispatcher& dispatcher);

  void ShowSettings();

  void ShowNewMapDialog();

  void ShowOpenMapDialog();

  void ShowAddTilesetDialog();

  void ShowRenameLayerDialog(LayerID id);

  void ShowAddPropertyDialog();

  void ShowRenamePropertyDialog(const std::string& name);

  void ShowChangePropertyTypeDialog(std::string name, PropertyType type);

  void ShowResizeMapDialog(usize currentRows, usize currentColumns);

  void ShowComponentEditor(const Model& model);

  void SetToolbarVisible(bool visible);

  [[nodiscard]] auto IsEditorFocused() const -> bool;
  [[nodiscard]] auto IsToolbarFocused() const -> bool;
  [[nodiscard]] auto IsViewportFocused() const -> bool;
  [[nodiscard]] auto IsLayerDockFocused() const -> bool;
  [[nodiscard]] auto IsTilesetDockFocused() const -> bool;
  [[nodiscard]] auto IsPropertiesDockFocused() const -> bool;
  [[nodiscard]] auto IsLogDockFocused() const -> bool;

  [[nodiscard]] auto IsTilesetDockHovered() const -> bool;

  [[nodiscard]] auto IsToolbarVisible() const -> bool;

  [[nodiscard]] auto GetTilesetViewWidth() const -> Maybe<float>;
  [[nodiscard]] auto GetTilesetViewHeight() const -> Maybe<float>;

 private:
  MenuBar mMenuBar;
  Toolbar mToolbar;
  TilesetDock mTilesetDock;
  LayerDock mLayerDock;
  PropertiesDock mPropertiesDock;
  ResizeMapDialog mResizeMapDialog;
};

}  // namespace Tactile