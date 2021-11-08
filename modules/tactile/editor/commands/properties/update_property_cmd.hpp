#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt.hpp>  // registry

#include "core/property_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

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
