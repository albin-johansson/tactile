#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "core/components/attribute_context.hpp"
#include "items/property_item_context_menu.hpp"
#include "tactile_def.hpp"

namespace tactile {

class PropertyTable final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

 private:
  PropertyItemContextMenuState mContextState;
  Maybe<std::string> mRenameTarget;
  Maybe<std::string> mChangeTypeTarget;

  void ShowCustomProperties(const entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const comp::attribute_context& context,
                            bool& isItemContextOpen);
};

}  // namespace tactile
