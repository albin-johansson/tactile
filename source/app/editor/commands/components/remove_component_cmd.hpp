#pragma once

#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class remove_component_cmd final : public command_base
{
 public:
  remove_component_cmd(registry_ref registry,
                       context_id contextId,
                       component_id componentId);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::remove_component;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId{};
  component_id mComponentId{};
  maybe<sys::remove_component_result> mSnapshot;
};

}  // namespace tactile
