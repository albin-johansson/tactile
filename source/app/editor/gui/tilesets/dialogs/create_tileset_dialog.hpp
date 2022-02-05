#pragma once

#include <array>       // array
#include <filesystem>  // path

#include "editor/gui/dialogs/dialog.hpp"

namespace tactile {

class CreateTilesetDialog final : public ADialog
{
 public:
  CreateTilesetDialog();

  void Open();

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool override;

 private:
  std::filesystem::path mFullImagePath;
  std::array<char, 100> mPathPreviewBuffer{};
  int mTileWidth{32};
  int mTileHeight{32};

  void ShowImageFileDialog();
};

}  // namespace tactile
