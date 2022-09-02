#pragma once

#include "core/cmd/command.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/layer/tile_format.hpp"
#include "core/map.hpp"

namespace tactile {

class SetTileFormatCompression final : public ICommand
{
 public:
  SetTileFormatCompression(Shared<Map> map, TileCompression compression);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> std::string override;

 private:
  Shared<Map> mMap;
  TileCompression mNewCompression;
  Maybe<TileCompression> mOldCompression;
};

}  // namespace tactile
