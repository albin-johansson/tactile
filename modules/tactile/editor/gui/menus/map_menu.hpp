#pragma once

#include <entt/entt.hpp>

namespace Tactile {

class Model;

class MapMenu final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

  void UpdateWindows(entt::dispatcher& dispatcher);

  void ShowAddTilesetDialog();

 private:
  bool mShowTilesetDialog{};
};

}  // namespace Tactile
