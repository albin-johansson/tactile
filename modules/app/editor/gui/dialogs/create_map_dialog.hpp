#pragma once

#include "dialog.hpp"

namespace tactile {

/// \addtogroup gui
/// \{

class CreateMapDialog final : public ADialog {
 public:
  CreateMapDialog();

  ~CreateMapDialog() override = default;

  void Open();

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool override;

 private:
  int mTileWidth{};
  int mTileHeight{};
  int mRows{};
  int mColumns{};
};

/// \} End of group gui

}  // namespace tactile
