#include "component_name_dialog.hpp"

#include "assert.hpp"
#include "core/systems/component_system.hpp"
#include "editor/model.hpp"

namespace Tactile {

auto AComponentNameDialog::Validate(const Model& model,
                                    const std::string_view input) const -> bool
{
  const auto* registry = model.GetActiveRegistry();
  TACTILE_ASSERT(registry);

  return !input.empty() && !Sys::IsComponentNameTaken(*registry, input);
}

}  // namespace Tactile