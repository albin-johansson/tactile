#pragma once

#include <memory>  // unique_ptr

#include <entt/entt.hpp>

#include "io/parser/parse_error.hpp"
#include "tactile_def.hpp"

namespace tactile {

class Model;
class Icons;

class MenuBar;
class Toolbar;
class TilesetDock;
class LayerDock;
class PropertiesDock;
class ComponentDock;
class LogDock;

class ResizeMapDialog;
class MapImportErrorDialog;

class WidgetManager final {
 public:
  WidgetManager();

  ~WidgetManager() noexcept;

  void Update(const Model& model, const Icons& icons, entt::dispatcher& dispatcher);

  void ShowSettings();

  void ShowNewMapDialog();

  void ShowOpenMapDialog();

  void ShowAddTilesetDialog();

  void ShowRenameLayerDialog(LayerID id);

  void ShowAddPropertyDialog();

  void ShowRenamePropertyDialog(const std::string& name);

  void ShowChangePropertyTypeDialog(std::string name, attribute_type type);

  void ShowResizeMapDialog(usize currentRows, usize currentColumns);

  void ShowMapImportErrorDialog(parsing::parse_error error);

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
  std::unique_ptr<MenuBar> mMenuBar;
  std::unique_ptr<Toolbar> mToolbar;
  std::unique_ptr<TilesetDock> mTilesetDock;
  std::unique_ptr<LayerDock> mLayerDock;
  std::unique_ptr<PropertiesDock> mPropertiesDock;
  std::unique_ptr<ComponentDock> mComponentDock;
  std::unique_ptr<LogDock> mLogDock;

  std::unique_ptr<ResizeMapDialog> mResizeMapDialog;
  std::unique_ptr<MapImportErrorDialog> mMapImportErrorDialog;
};

}  // namespace tactile