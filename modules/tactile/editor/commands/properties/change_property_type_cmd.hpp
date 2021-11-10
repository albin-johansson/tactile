#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

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
