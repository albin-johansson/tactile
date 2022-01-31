#pragma once

#include <vector>  // vector

#include <entt/entt.hpp>  // registry

#include "core/tile_position.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class BucketCmd final : public command_base {
 public:
  BucketCmd(registry_ref registry, tile_position origin, tile_id replacement);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override { return CommandId::Bucket; }

 private:
  registry_ref mRegistry;
  layer_id mLayer;
  tile_position mOrigin;
  tile_id mReplacement;
  maybe<tile_id> mTarget;
  std::vector<tile_position> mPositions;
};

/// \} End of group commands

}  // namespace tactile
