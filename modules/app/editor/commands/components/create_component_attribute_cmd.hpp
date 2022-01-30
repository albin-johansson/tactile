#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class CreateComponentAttributeCmd final : public ACommand {
 public:
  CreateComponentAttributeCmd(RegistryRef registry, component_id id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::CreateComponentDefAttribute;
  }

 private:
  RegistryRef mRegistry;
  component_id mComponentId{};
  std::string mName;
};

}  // namespace tactile