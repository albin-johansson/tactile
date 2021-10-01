#pragma once

#include <entt.hpp>  // registry
#include <string>    // string

#include "common/context_id.hpp"
#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/properties/property_type.hpp"
#include "core/properties/property_value.hpp"

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
