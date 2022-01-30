#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile_def.hpp"

namespace tactile {

class SetObjectVisibilityCmd final : public AObjectCmd {
 public:
  SetObjectVisibilityCmd(RegistryRef registry, object_id id, bool visible);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetObjectVisibility;
  }

 private:
  bool mVisible;
  maybe<bool> mPreviousVisibility;
};

}  // namespace tactile
