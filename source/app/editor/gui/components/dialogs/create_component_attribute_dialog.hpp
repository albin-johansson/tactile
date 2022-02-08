#pragma once

#include "core/components/component.hpp"
#include "editor/gui/dialogs/string_input_dialog.hpp"

namespace tactile {

class create_component_attribute_dialog final : public string_input_dialog
{
 public:
  create_component_attribute_dialog();

  void show(component_id id);

 protected:
  [[nodiscard]] auto validate(const Model& model, std::string_view input) const
      -> bool override;

  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  component_id mComponentId{};
};

}  // namespace tactile
