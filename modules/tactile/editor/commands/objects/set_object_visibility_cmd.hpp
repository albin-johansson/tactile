#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"

namespace Tactile {

class SetObjectVisibilityCmd final : public AObjectCmd
{
 public:
  SetObjectVisibilityCmd(Ref<entt::registry> registry, ObjectID id, bool visible);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::SetObjectVisibility;
  }

 private:
  bool mVisible;
  Maybe<bool> mPreviousVisibility;
};

}  // namespace Tactile
