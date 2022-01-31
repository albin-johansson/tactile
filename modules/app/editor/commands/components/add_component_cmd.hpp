#pragma once

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AddComponentCmd final : public command_base {
 public:
  AddComponentCmd(registry_ref registry, context_id contextId, component_id componentId);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::add_component;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId{};
  component_id mComponentId{};
};

}  // namespace tactile