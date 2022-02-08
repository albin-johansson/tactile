#include "component_name_dialog.hpp"

#include "core/systems/component_system.hpp"
#include "editor/model.hpp"

namespace tactile {

auto component_name_dialog::validate(const Model& model, std::string_view input) const
    -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() && !sys::is_component_name_taken(registry, input);
}

}  // namespace tactile