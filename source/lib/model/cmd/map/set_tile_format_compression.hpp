// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/tile_format.hpp"
#include "core/map.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile {

class SetTileFormatCompression final : public Command {
 public:
  SetTileFormatCompression(Shared<Map> map, TileCompression compression);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  TileCompression mNewCompression;
  Maybe<TileCompression> mOldCompression;
};

}  // namespace tactile
