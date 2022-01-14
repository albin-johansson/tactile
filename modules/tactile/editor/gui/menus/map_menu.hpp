#pragma once

#include <memory>  // unique_ptr

#include <entt/entt.hpp>

namespace Tactile {

class Model;
class CreateTilesetDialog;

class MapMenu final {
 public:
  MapMenu();

  ~MapMenu() noexcept;

  void Update(const Model& model, entt::dispatcher& dispatcher);

  void ShowAddTilesetDialog();

 private:
  std::unique_ptr<CreateTilesetDialog> mCreateTilesetDialog;
};

}  // namespace Tactile
