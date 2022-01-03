#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class CreateComponentAttributeCmd final : public ACommand {
 public:
  CreateComponentAttributeCmd(RegistryRef registry, ComponentID id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::CreateComponentDefAttribute;
  }

 private:
  RegistryRef mRegistry;
  ComponentID mComponentId{};
  std::string mName;
};

}  // namespace Tactile