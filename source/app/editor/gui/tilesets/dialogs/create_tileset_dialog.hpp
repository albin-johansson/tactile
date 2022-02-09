#pragma once

#include <array>       // array
#include <filesystem>  // path

#include "editor/gui/dialogs/dialog.hpp"

namespace tactile {

class CreateTilesetDialog final : public dialog_base
{
 public:
  CreateTilesetDialog();

  void Open();

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_current_input_valid(const document_model& model) const
      -> bool override;

 private:
  std::filesystem::path mFullImagePath;
  std::array<char, 100> mPathPreviewBuffer{};
  int mTileWidth{32};
  int mTileHeight{32};

  void ShowImageFileDialog();
};

}  // namespace tactile
