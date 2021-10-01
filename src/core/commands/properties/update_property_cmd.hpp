#pragma once

#include <entt.hpp>  // registry
#include <string>    // string

#include "common/context_id.hpp"
#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/properties/property_value.hpp"

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
