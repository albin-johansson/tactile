// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/tile_format.hpp"
#include "core/map.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile {

class SetTileFormatEncoding final : public Command
{
 public:
  SetTileFormatEncoding(Shared<Map> map, TileEncoding encoding);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  TileEncoding mNewEncoding;
  Maybe<TileEncoding> mOldEncoding;
  Maybe<CompressionType> mOldCompression;
};

}  // namespace tactile
