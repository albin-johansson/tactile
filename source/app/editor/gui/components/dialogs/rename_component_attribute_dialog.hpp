#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/gui/dialogs/string_input_dialog.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Used to rename an attribute in a component definition.
 *
 * \ingroup gui
 */
class rename_component_attribute_dialog final : public string_input_dialog
{
 public:
  rename_component_attribute_dialog();

  void show(std::string previousName, component_id id);

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto validate(const Model& model, std::string_view input) const
      -> bool override;

 private:
  maybe<component_id> mComponentId;
};

}  // namespace tactile