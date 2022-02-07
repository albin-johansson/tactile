#pragma once

#include <array>  // array

#include "core/attribute.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class AddPropertyDialog final : public ADialog
{
 public:
  AddPropertyDialog();

  ~AddPropertyDialog() override = default;

  void Open();

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsCurrentInputValid(const Model& model) const -> bool override;

 private:
  std::array<char, 128> mNameBuffer{};
  attribute_type mPropertyType{attribute_type::string};
};

}  // namespace tactile
