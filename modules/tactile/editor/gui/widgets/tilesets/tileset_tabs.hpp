#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

#include "tileset_view.hpp"

namespace Tactile {

class TilesetTabWidget final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

  [[nodiscard]] auto GetTilesetView() const -> const TilesetView&;

 private:
  TilesetView mTilesetView;
};

}  // namespace Tactile
