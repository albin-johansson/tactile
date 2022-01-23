#pragma once

#include <tactile_def.hpp>

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class AddComponentCmd final : public ACommand {
 public:
  AddComponentCmd(RegistryRef registry, ContextID contextId, ComponentID componentId);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddComponent;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId{};
  ComponentID mComponentId{};
};

}  // namespace tactile