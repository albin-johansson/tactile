#pragma once

#include <entt.hpp>  // registry
#include <string>    // string

#include "common/context_id.hpp"
#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "core/property_type.hpp"
#include "core/property_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class ChangePropertyTypeCmd final : public ACommand
{
 public:
  ChangePropertyTypeCmd(Ref<entt::registry> registry,
                        std::string name,
                        PropertyType type);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ChangePropertyType;
  }

 private:
  Ref<entt::registry> mRegistry;
  ContextID mContextId;
  std::string mName;
  PropertyType mPropertyType;
  Maybe<PropertyValue> mPreviousValue;
};

}  // namespace Tactile
