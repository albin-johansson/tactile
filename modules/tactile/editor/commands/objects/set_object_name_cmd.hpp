#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"

namespace Tactile {

class SetObjectNameCmd final : public AObjectCmd {
 public:
  SetObjectNameCmd(Ref<entt::registry> registry, ObjectID id, std::string name);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetObjectName;
  }

 private:
  std::string mNewName;
  Maybe<std::string> mOldName;
};

}  // namespace Tactile
