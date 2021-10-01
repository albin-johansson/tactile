#pragma once

#include <entt.hpp>  // registry
#include <string>    // string

#include "common/context_id.hpp"
#include "common/ref.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"

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
  std::string mNewName;
  std::string mOldName;
};

}  // namespace Tactile
