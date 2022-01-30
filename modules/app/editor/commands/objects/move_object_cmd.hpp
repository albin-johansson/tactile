#pragma once

#include <entt/entt.hpp>  // registry

#include "editor/commands/command_id.hpp"
#include "object_cmd.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class MoveObjectCmd final : public AObjectCmd {
 public:
  MoveObjectCmd(RegistryRef registry,
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

}  // namespace tactile
