#pragma once

#include <tactile_def.hpp>

#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class ResetComponentCmd final : public ACommand {
 public:
  ResetComponentCmd(RegistryRef registry, ContextID contextId, ComponentID componentId);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ResetComponent;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId{};
  ComponentID mComponentId{};
  Maybe<sys::ResetComponentResult> mSnapshot;
};

}  // namespace tactile