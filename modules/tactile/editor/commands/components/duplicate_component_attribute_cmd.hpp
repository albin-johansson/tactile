#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class DuplicateComponentAttributeCmd final : public ACommand {
 public:
  DuplicateComponentAttributeCmd(RegistryRef registry,
                                 ComponentID componentId,
                                 std::string attribute);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::DuplicateComponentAttribute;
  }

 private:
  RegistryRef mRegistry;
  ComponentID mComponentId{};
  std::string mAttributeName;
  Maybe<std::string> mDuplicatedName;
};

}  // namespace tactile