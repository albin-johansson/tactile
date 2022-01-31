#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RenamePropertyCmd final : public command_base {
 public:
  RenamePropertyCmd(registry_ref registry, std::string oldName, std::string newName);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::RenameProperty;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mOldName;
  std::string mNewName;
};

}  // namespace tactile
