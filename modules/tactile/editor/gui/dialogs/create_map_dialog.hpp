#pragma once

#include "dialog.hpp"

namespace Tactile {

/// \addtogroup gui
/// \{

class CreateMapDialog final : public ADialog {
 public:
  CreateMapDialog();

  ~CreateMapDialog() override = default;

  void Open();

 protected:
  void UpdateContents(const entt::registry& registry,
                      entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const entt::registry& registry) const
      -> bool override;

 private:
  int mTileWidth{};
  int mTileHeight{};
  int mRows{};
  int mColumns{};
};

/// \} End of group gui

}  // namespace Tactile
