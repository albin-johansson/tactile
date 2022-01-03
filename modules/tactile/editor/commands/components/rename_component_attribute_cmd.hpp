#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class RenameComponentAttributeCmd final : public ACommand {
 public:
  RenameComponentAttributeCmd(RegistryRef registry,
                              ComponentID id,
                              std::string previousName,
                              std::string updatedName);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameComponentAttribute;
  }

 private:
  RegistryRef mRegistry;
  ComponentID mComponentId{};
  std::string mPreviousName;
  std::string mUpdatedName;
};

}  // namespace Tactile