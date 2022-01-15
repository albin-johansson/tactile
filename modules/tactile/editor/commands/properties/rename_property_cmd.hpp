#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class RenamePropertyCmd final : public ACommand {
 public:
  RenamePropertyCmd(RegistryRef registry, std::string oldName, std::string newName);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameProperty;
  }

 private:
  RegistryRef mRegistry;
  ContextID mContextId;
  std::string mOldName;
  std::string mNewName;
};

}  // namespace Tactile
