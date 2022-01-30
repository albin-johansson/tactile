#pragma once

#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemoveComponentCmd final : public ACommand {
 public:
  RemoveComponentCmd(RegistryRef registry,
                     context_id contextId,
                     component_id componentId);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveComponent;
  }

 private:
  RegistryRef mRegistry;
  context_id mContextId{};
  component_id mComponentId{};
  maybe<sys::remove_component_result> mSnapshot;
};

}  // namespace tactile
