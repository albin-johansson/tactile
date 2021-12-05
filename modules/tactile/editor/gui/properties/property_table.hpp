#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt/entt.hpp>

#include "core/components/property_context.hpp"
#include "items/property_item_context_menu.hpp"

namespace Tactile {

class PropertyTable final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

 private:
  PropertyItemContextMenuState mContextState;
  Maybe<std::string> mRenameTarget;
  Maybe<std::string> mChangeTypeTarget;

  void ShowCustomProperties(const entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const PropertyContext& context,
                            bool& isItemContextOpen);
};

}  // namespace Tactile
