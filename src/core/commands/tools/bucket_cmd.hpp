#pragma once

#include <entt.hpp>  // registry
#include <vector>    // vector

#include "aliases/layer_id.hpp"
#include "aliases/maybe.hpp"
#include "aliases/ref.hpp"
#include "aliases/tile_id.hpp"
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
