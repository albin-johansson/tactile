#pragma once

#include <string>  // string

#include <entt.hpp>  // registry

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

#include "tactile/editor/commands/command.hpp"
#include "tactile/editor/commands/command_id.hpp"

namespace Tactile {

class UpdatePropertyCmd final : public ACommand
{
 public:
  UpdatePropertyCmd(Ref<entt::registry> registry, std::string name, PropertyValue value);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::UpdateProperty;
  }

 private:
  Ref<entt::registry> mRegistry;
  ContextID mContextId;
  std::string mName;
  PropertyValue mNewValue;
  Maybe<PropertyValue> mOldValue;
};

}  // namespace Tactile
