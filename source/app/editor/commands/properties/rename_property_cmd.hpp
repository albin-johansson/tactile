#pragma once

#include <string>  // string

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class rename_property_cmd final : public command_base
{
 public:
  rename_property_cmd(registry_ref registry, std::string oldName, std::string newName);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::rename_property;
  }

 private:
  registry_ref mRegistry;
  context_id mContextId;
  std::string mOldName;
  std::string mNewName;
};

}  // namespace tactile
