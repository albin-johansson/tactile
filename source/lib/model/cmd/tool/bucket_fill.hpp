// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/map.hpp"
#include "core/tile/tile_pos.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile::cmd {

class BucketFill final : public Command {
 public:
  BucketFill(Shared<Map> map,
             const UUID& layer_id,
             const TilePos& origin,
             TileID replacement);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  UUID mLayerId {};
  TilePos mOrigin;
  TileID mReplacement {};
  Maybe<TileID> mTarget;
  Vector<TilePos> mPositions;
};

}  // namespace tactile::cmd
