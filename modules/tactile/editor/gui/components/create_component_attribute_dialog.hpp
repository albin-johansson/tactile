#pragma once

#include "core/components/component.hpp"
#include "editor/gui/dialogs/string_input_dialog.hpp"

namespace Tactile {

class CreateComponentAttributeDialog final : public AStringInputDialog {
 public:
  CreateComponentAttributeDialog();

  void Open(ComponentID id);

 protected:
  [[nodiscard]] auto Validate(const Model& model, std::string_view input) const
      -> bool override;

  void OnAccept(entt::dispatcher& dispatcher) override;

 private:
  ComponentID mComponentId{};
};

}  // namespace Tactile
