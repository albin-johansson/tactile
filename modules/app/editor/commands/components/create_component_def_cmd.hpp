#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class CreateComponentDefCmd final : public ACommand {
 public:
  CreateComponentDefCmd(RegistryRef registry, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::CreateComponentDef;
  }

 private:
  RegistryRef mRegistry;
  std::string mName;
  maybe<component_id> mComponentId;
};

}  // namespace tactile