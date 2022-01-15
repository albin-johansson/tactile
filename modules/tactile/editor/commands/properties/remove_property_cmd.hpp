#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/property_value.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class RemovePropertyCmd final : public ACommand {
 public:
  RemovePropertyCmd(RegistryRef registry, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveProperty;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId;
  std::string mName;
  Maybe<PropertyValue> mPreviousValue;
};

}  // namespace Tactile
