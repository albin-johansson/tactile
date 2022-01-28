#pragma once

#include <array>  // array

#include "editor/gui/dialogs/dialog.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AddPropertyDialog final : public ADialog {
 public:
  AddPropertyDialog();

  ~AddPropertyDialog() override = default;

  using ADialog::Show;

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool override;

 private:
  std::array<char, 128> mNameBuffer{};
  PropertyType mPropertyType{PropertyType::String};
};

}  // namespace tactile
