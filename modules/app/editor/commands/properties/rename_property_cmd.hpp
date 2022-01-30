#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

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
  context_id mContextId;
  std::string mOldName;
  std::string mNewName;
};

}  // namespace tactile
