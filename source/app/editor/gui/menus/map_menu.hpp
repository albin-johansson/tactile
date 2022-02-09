#pragma once

#include <memory>  // unique_ptr

#include <entt/entt.hpp>

namespace tactile {

class document_model;
class CreateTilesetDialog;

class MapMenu final
{
 public:
  MapMenu();

  ~MapMenu() noexcept;

  void Update(const document_model& model, entt::dispatcher& dispatcher);

  void ShowAddTilesetDialog();

 private:
  std::unique_ptr<CreateTilesetDialog> mCreateTilesetDialog;
};

}  // namespace tactile
