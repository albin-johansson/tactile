#pragma once

#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class ResetComponentCmd final : public command_base {
 public:
  ResetComponentCmd(registry_ref registry, context_id contextId, component_id componentId);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::reset_component;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId{};
  component_id mComponentId{};
  maybe<sys::reset_component_result> mSnapshot;
};

}  // namespace tactile