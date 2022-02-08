#pragma once

#include "dialog.hpp"

namespace tactile {

/// \addtogroup gui
/// \{

class CreateMapDialog final : public dialog_base
{
 public:
  CreateMapDialog();

  ~CreateMapDialog() override = default;

  void Open();

 protected:
  void on_update(const Model& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_current_input_valid(const Model& model) const -> bool override;

 private:
  int mTileWidth{};
  int mTileHeight{};
  int mRows{};
  int mColumns{};
};

/// \} End of group gui

}  // namespace tactile
