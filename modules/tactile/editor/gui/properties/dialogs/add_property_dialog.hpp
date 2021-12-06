#pragma once

#include <array>  // array

#include <tactile_def.hpp>

#include "editor/gui/dialogs/dialog.hpp"

namespace Tactile {

class AddPropertyDialog final : public ADialog {
 public:
  AddPropertyDialog();

  ~AddPropertyDialog() override = default;

  using ADialog::Show;

 protected:
  void UpdateContents(const entt::registry& registry,
                      entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const entt::registry& registry) const
      -> bool override;

 private:
  std::array<char, 128> mNameBuffer{};
  PropertyType mPropertyType{PropertyType::String};
};

}  // namespace Tactile
