#pragma once

#include "editor/gui/dialogs/string_input_dialog.hpp"

namespace tactile {

class RenamePropertyDialog final : public string_input_dialog
{
 public:
  RenamePropertyDialog();

  ~RenamePropertyDialog() override = default;

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto validate(const Model& model, std::string_view input) const
      -> bool override;
};

}  // namespace tactile
