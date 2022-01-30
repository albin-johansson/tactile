#pragma once

#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ResetComponentCmd final : public ACommand {
 public:
  ResetComponentCmd(RegistryRef registry, context_id contextId, component_id componentId);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::ResetComponent;
  }

 private:
  RegistryRef mRegistry;
  context_id mContextId{};
  component_id mComponentId{};
  maybe<sys::reset_component_result> mSnapshot;
};

}  // namespace tactile