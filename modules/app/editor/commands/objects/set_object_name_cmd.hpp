#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetObjectNameCmd final : public AObjectCmd {
 public:
  SetObjectNameCmd(registry_ref registry, object_id id, std::string name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const command_base& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::SetObjectName;
  }

 private:
  std::string mNewName;
  maybe<std::string> mOldName;
};

}  // namespace tactile
