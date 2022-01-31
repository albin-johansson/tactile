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

class BucketCmd final : public ACommand {
 public:
  BucketCmd(RegistryRef registry, tile_position origin, tile_id replacement);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override { return CommandId::Bucket; }

 private:
  RegistryRef mRegistry;
  layer_id mLayer;
  tile_position mOrigin;
  tile_id mReplacement;
  maybe<tile_id> mTarget;
  std::vector<tile_position> mPositions;
};

/// \} End of group commands

}  // namespace tactile