// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/map.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile::cmd {

class FixMapTiles final : public Command {
 public:
  explicit FixMapTiles(Shared<Map> map);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  Map::FixTilesResult mInvalidLayerTiles;
};

}  // namespace tactile::cmd
