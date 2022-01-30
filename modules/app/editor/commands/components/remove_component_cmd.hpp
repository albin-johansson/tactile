#pragma once

#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemoveComponentCmd final : public ACommand {
 public:
  RemoveComponentCmd(RegistryRef registry, ContextID contextId, ComponentID componentId);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveComponent;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId{};
  ComponentID mComponentId{};
  Maybe<sys::remove_component_result> mSnapshot;
};

}  // namespace tactile
