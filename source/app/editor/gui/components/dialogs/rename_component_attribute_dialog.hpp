#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/gui/dialogs/string_input_dialog.hpp"
#include "tactile.hpp"

namespace tactile {

class RenameComponentAttributeDialog final : public AStringInputDialog {
 public:
  RenameComponentAttributeDialog();

  void Open(std::string previousName, component_id id);

 protected:
  [[nodiscard]] auto Validate(const Model& model, std::string_view input) const
      -> bool override;
  void OnAccept(entt::dispatcher& dispatcher) override;

 private:
  maybe<component_id> mComponentId;
};

}  // namespace tactile