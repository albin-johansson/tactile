#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class RenamePropertyCmd final : public ACommand
{
 public:
  RenamePropertyCmd(Ref<entt::registry> registry,
                    std::string oldName,
                    std::string newName);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RenameProperty;
  }

 private:
  Ref<entt::registry> mRegistry;
  ContextID mContextId;
  std::string mOldName;
  std::string mNewName;
};

}  // namespace Tactile
