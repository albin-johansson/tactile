#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class MoveObjectCmd final : public AObjectCmd {
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
  float mOldX;
  float mOldY;
  float mNewX;
  float mNewY;
};

/// \} End of group commands

}  // namespace Tactile
