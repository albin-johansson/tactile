#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "menus/menu_bar.hpp"
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
};

}  // namespace Tactile