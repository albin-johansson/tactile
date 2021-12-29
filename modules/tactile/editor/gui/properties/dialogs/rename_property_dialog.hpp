#pragma once

#include "editor/gui/dialogs/rename_dialog.hpp"

namespace Tactile {

class RenamePropertyDialog final : public ARenameDialog {
 public:
  RenamePropertyDialog();

  ~RenamePropertyDialog() override = default;

 protected:
  void OnAccept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto Validate(const Model& model, std::string_view input) const
      -> bool override;
};

}  // namespace Tactile
