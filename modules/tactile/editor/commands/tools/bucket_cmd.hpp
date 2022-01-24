#pragma once

#include <vector>  // vector

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/tile_position.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class BucketCmd final : public ACommand {
 public:
  BucketCmd(RegistryRef registry, tile_position origin, TileID replacement);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override { return CommandId::Bucket; }

 private:
  RegistryRef mRegistry;
  LayerID mLayer;
  tile_position mOrigin;
  TileID mReplacement;
  Maybe<TileID> mTarget;
  std::vector<tile_position> mPositions;
};

/// \} End of group commands

}  // namespace tactile
