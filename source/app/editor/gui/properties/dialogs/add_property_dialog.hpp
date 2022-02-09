#pragma once

#include <array>  // array

#include "core/attribute.hpp"
#include "editor/gui/dialogs/dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class AddPropertyDialog final : public dialog_base
{
 public:
  AddPropertyDialog();

  ~AddPropertyDialog() override = default;

  void Open();

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_current_input_valid(const document_model& model) const
      -> bool override;

 private:
  std::array<char, 128> mNameBuffer{};
  attribute_type mPropertyType{attribute_type::string};
};

}  // namespace tactile
