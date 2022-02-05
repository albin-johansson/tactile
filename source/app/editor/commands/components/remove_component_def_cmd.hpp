#pragma once

#include "core/components/component.hpp"
#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class RemoveComponentDefCmd final : public command_base {
 public:
  RemoveComponentDefCmd(registry_ref registry, component_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::remove_component_def;
  }

 private:
  registry_ref mRegistry;
  component_id mComponentId{};
  maybe<sys::remove_component_def_result> mSnapshot;
};

}  // namespace tactile