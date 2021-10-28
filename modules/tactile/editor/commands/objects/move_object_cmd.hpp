#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class MoveObjectCmd final : public ACommand
{
 public:
  MoveObjectCmd(Ref<entt::registry> registry,
                ObjectID id,
                float oldX,
                float oldY,
                float newX,
                float newY);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::MoveObject;
  }

 private:
  Ref<entt::registry> mRegistry;
  ObjectID mObject;
  float mOldX;
  float mOldY;
  float mNewX;
  float mNewY;

  void SetObjectPosition(float x, float y);
};

/// \} End of group commands

}  // namespace Tactile
