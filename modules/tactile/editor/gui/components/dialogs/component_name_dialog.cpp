#include "component_name_dialog.hpp"

#include "core/systems/component_system.hpp"
#include "editor/model.hpp"

namespace tactile {

auto AComponentNameDialog::Validate(const Model& model,
                                    const std::string_view input) const -> bool
{
  const auto& registry = model.GetActiveRegistryRef();
  return !input.empty() && !sys::IsComponentNameTaken(registry, input);
}

}  // namespace tactile