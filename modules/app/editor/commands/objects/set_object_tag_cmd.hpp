#pragma once

#include <string>  // string

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetObjectTagCmd final : public AObjectCmd {
 public:
  SetObjectTagCmd(RegistryRef registry, object_id id, std::string tag);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto MergeWith(const ACommand& cmd) -> bool override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetObjectTag;
  }

 private:
  std::string mNewTag;
  maybe<std::string> mOldTag;
};

}  // namespace tactile
