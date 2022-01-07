#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "tileset_tabs.hpp"

namespace Tactile {

class TilesetDock final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

  [[nodiscard]] auto IsFocused() const noexcept -> bool { return mHasFocus; }

  [[nodiscard]] auto IsHovered() const noexcept -> bool { return mWindowContainsMouse; }

  [[nodiscard]] auto GetTilesetView() const -> const TilesetView&;

 private:
  TilesetTabWidget mTabWidget;
  bool mHasFocus{};
  bool mWindowContainsMouse{};

  void ResetState();
};

}  // namespace Tactile
