#pragma once

#include <vector>  // vector

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/map_position.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class BucketCmd final : public ACommand {
 public:
  BucketCmd(RegistryRef registry, MapPosition origin, TileID replacement);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override { return CommandId::Bucket; }

 private:
  RegistryRef mRegistry;
  LayerID mLayer;
  MapPosition mOrigin;
  TileID mReplacement;
  Maybe<TileID> mTarget;
  std::vector<MapPosition> mPositions;
};

/// \} End of group commands

}  // namespace tactile
