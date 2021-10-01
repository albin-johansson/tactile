#pragma once

#include <entt.hpp>  // registry
#include <vector>    // vector

#include "common/layer_id.hpp"
#include "common/maybe.hpp"
#include "common/ref.hpp"
#include "common/tile_id.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/map_position.hpp"

namespace Tactile {

/// \addtogroup commands
/// \{

class BucketCmd final : public ACommand
{
 public:
  BucketCmd(Ref<entt::registry> registry, MapPosition origin, TileID replacement);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::Bucket;
  }

 private:
  Ref<entt::registry> mRegistry;
  LayerID mLayer;
  MapPosition mOrigin;
  TileID mReplacement;
  Maybe<TileID> mTarget;
  std::vector<MapPosition> mPositions;
};

/// \} End of group commands

}  // namespace Tactile
