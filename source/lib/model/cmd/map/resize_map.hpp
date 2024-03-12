// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/map.hpp"
#include "core/tile/tile_extent.hpp"
#include "model/cmd/command.hpp"
#include "model/cmd/map/map_command_cache.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"

namespace tactile::cmd {

class ResizeMap final : public Command {
 public:
  ResizeMap(Shared<Map> map, TileExtent extent);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  TileExtent mNewExtent;
  Maybe<TileExtent> mOldExtent {};
  MapCommandCache mCache;

  [[nodiscard]] auto is_lossy_resize() const -> bool;
};

}  // namespace tactile::cmd
