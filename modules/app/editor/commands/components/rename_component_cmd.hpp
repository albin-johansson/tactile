#pragma once

#include <string>  // string

#include "core/components/component.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RenameComponentCmd final : public ACommand {
 public:
  RenameComponentCmd(RegistryRef registry, component_id id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameComponent;
  }

 private:
  RegistryRef mRegistry;
  component_id mComponentId{};
  std::string mUpdatedName;
  maybe<std::string> mPreviousName;
};

}  // namespace tactile