#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

struct Object;

class SetObjectVisibilityCmd final : public ACommand
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
  Ref<entt::registry> mRegistry;
  ObjectID mObjectId;
  bool mVisible;
  Maybe<bool> mPreviousVisibility;

  [[nodiscard]] auto GetObject() -> Object&;
};

}  // namespace Tactile
