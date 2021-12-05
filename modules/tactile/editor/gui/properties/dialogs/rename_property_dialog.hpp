#pragma once

#include "editor/gui/dialogs/rename_dialog.hpp"

namespace Tactile {

class RenamePropertyDialog final : public ARenameDialog {
 public:
  RenamePropertyDialog();

  ~RenamePropertyDialog() override = default;

 protected:
  void OnAccept(entt::dispatcher& dispatcher, const std::string& input) override;

  [[nodiscard]] auto IsInputValid(const entt::registry& registry, std::string_view input)
      -> bool override;
};

}  // namespace Tactile
