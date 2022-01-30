#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetObjectNameCmd final : public AObjectCmd {
 public:
  SetObjectNameCmd(RegistryRef registry, object_id id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetObjectName;
  }

 private:
  std::string mNewName;
  maybe<std::string> mOldName;
};

}  // namespace tactile
