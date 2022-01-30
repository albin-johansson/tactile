#pragma once

#include "core/components/component.hpp"
#include "core/systems/component_system.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemoveComponentDefCmd final : public ACommand {
 public:
  RemoveComponentDefCmd(RegistryRef registry, component_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveComponentDef;
  }

 private:
  RegistryRef mRegistry;
  component_id mComponentId{};
  maybe<sys::remove_component_def_result> mSnapshot;
};

}  // namespace tactile