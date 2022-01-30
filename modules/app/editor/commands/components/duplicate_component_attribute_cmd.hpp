#pragma once

#include <string>  // string

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class DuplicateComponentAttributeCmd final : public ACommand {
 public:
  DuplicateComponentAttributeCmd(RegistryRef registry,
                                 component_id componentId,
                                 std::string attribute);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::DuplicateComponentAttribute;
  }

 private:
  RegistryRef mRegistry;
  component_id mComponentId{};
  std::string mAttributeName;
  maybe<std::string> mDuplicatedName;
};

}  // namespace tactile