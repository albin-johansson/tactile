#pragma once

#include <vector>  // vector

#include "aliases/maybe.hpp"
#include "aliases/not_null.hpp"
#include "aliases/tile_id.hpp"
#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/map/map_position.hpp"

namespace Tactile {

class MapDocument;

class BucketCmd final : public ACommand
{
 public:
  BucketCmd(NotNull<MapDocument*> document, MapPosition origin, tile_id replacement);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::Bucket;
  }

 private:
  MapDocument* mDocument{};
  MapPosition mOrigin;
  tile_id mReplacement;
  Maybe<tile_id> mTarget;
  std::vector<MapPosition> mPositions;
};

}  // namespace Tactile
