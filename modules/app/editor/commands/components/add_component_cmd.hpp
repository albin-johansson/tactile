#pragma once

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AddComponentCmd final : public ACommand {
 public:
  AddComponentCmd(RegistryRef registry, context_id contextId, component_id componentId);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddComponent;
  }

 private:
  RegistryRef mRegistry;
  context_id mContextId{};
  component_id mComponentId{};
};

}  // namespace tactile