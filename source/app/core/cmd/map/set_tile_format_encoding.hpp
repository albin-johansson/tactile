#pragma once

#include "core/cmd/command.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/layer/tile_format.hpp"
#include "core/map.hpp"

namespace tactile {

class SetTileFormatEncoding final : public ICommand
{
 public:
  SetTileFormatEncoding(Shared<Map> map, TileEncoding encoding);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> std::string override;

 private:
  Shared<Map>            mMap;
  TileEncoding           mNewEncoding;
  Maybe<TileEncoding>    mOldEncoding;
  Maybe<TileCompression> mOldCompression;
};

}  // namespace tactile
