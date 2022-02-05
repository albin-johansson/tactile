#pragma once

#include "core/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class eraser_sequence_cmd final : public command_base
{
 public:
  eraser_sequence_cmd(registry_ref registry, TileCache&& oldState);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::eraser_sequence;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayer;
  TileCache mOldState;
};

/// \} End of group commands

}  // namespace tactile
