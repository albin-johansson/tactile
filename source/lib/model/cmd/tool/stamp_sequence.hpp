// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "common/type/tile_cache.hpp"
#include "core/map.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile::cmd {

class StampSequence final : public Command {
 public:
  StampSequence(Shared<Map> map,
                const UUID& layer_id,
                TileCache old_state,
                TileCache new_state);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  UUID mLayerId {};
  TileCache mOldState;
  TileCache mNewState;

  void apply_sequence(const TileCache& cache);
};

}  // namespace tactile::cmd
