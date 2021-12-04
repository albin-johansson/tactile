#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "tilesets/tileset_dock.hpp"

namespace Tactile {

class Model;
class Icons;

class WidgetManager final
{
 public:
  void Update(const Model& model, const Icons& icons, entt::dispatcher& dispatcher);

  [[nodiscard]] auto IsEditorFocused() const -> bool;
  [[nodiscard]] auto IsToolbarFocused() const -> bool;
  [[nodiscard]] auto IsViewportFocused() const -> bool;
  [[nodiscard]] auto IsLayerDockFocused() const -> bool;
  [[nodiscard]] auto IsTilesetDockFocused() const -> bool;
  [[nodiscard]] auto IsPropertiesDockFocused() const -> bool;
  [[nodiscard]] auto IsLogDockFocused() const -> bool;

  [[nodiscard]] auto IsTilesetDockHovered() const -> bool;

  [[nodiscard]] auto GetTilesetDock() const -> const TilesetDock&;

  [[nodiscard]] auto GetTilesetViewWidth() const -> Maybe<float>;
  [[nodiscard]] auto GetTilesetViewHeight() const -> Maybe<float>;

 private:
  TilesetDock mTilesetDock;
};

}  // namespace Tactile