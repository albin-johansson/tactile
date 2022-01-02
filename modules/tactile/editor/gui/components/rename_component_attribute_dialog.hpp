#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "core/components/component.hpp"
#include "editor/gui/dialogs/string_input_dialog.hpp"

namespace Tactile {

class RenameComponentAttributeDialog final : public AStringInputDialog {
 public:
  RenameComponentAttributeDialog();

  void Open(std::string previousName, ComponentID id);

 protected:
  [[nodiscard]] auto Validate(const Model& model, std::string_view input) const
      -> bool override;
  void OnAccept(entt::dispatcher& dispatcher) override;

 private:
  Maybe<ComponentID> mComponentId;
};

}  // namespace Tactile