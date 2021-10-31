#pragma once

#include <string>  // string

#include <tactile-base/property_type.hpp>
#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class RemovePropertyCmd final : public ACommand
{
 public:
  RemovePropertyCmd(Ref<entt::registry> registry, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveProperty;
  }

 private:
  Ref<entt::registry> mRegistry;
  ContextID mContextId;
  std::string mName;
  Maybe<PropertyValue> mPreviousValue;
};

}  // namespace Tactile
