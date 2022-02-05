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
  MoveObjectCmd(registry_ref registry,
                object_id id,
                float oldX,
                float oldY,
                float newX,
                float newY);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::move_object;
  }

 private:
  float mOldX;
  float mOldY;
  float mNewX;
  float mNewY;
};

/// \} End of group commands

}  // namespace tactile