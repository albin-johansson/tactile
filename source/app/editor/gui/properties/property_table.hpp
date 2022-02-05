#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "core/components/attribute_context.hpp"
#include "items/property_item_context_menu.hpp"
#include "tactile.hpp"

namespace tactile {

class PropertyTable final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

 private:
  PropertyItemContextMenuState mContextState;
  maybe<std::string> mRenameTarget;
  maybe<std::string> mChangeTypeTarget;

  void ShowCustomProperties(const entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const comp::attribute_context& context,
                            bool& isItemContextOpen);
};

}  // namespace tactile
