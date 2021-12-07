#pragma once

#include "dialog.hpp"

namespace Tactile {

/// \addtogroup gui
/// \{

class AddMapDialog final : public ADialog {
 public:
  AddMapDialog();

  ~AddMapDialog() override = default;

  void Open();

 protected:
  void UpdateContents(const entt::registry& registry,
                      entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

 private:
  int mTileWidth{};
  int mTileHeight{};
};

/// \} End of group gui

}  // namespace Tactile
