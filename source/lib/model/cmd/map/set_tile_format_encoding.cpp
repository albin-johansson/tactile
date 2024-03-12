// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_tile_format_encoding.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

SetTileFormatEncoding::SetTileFormatEncoding(Shared<Map> map, const TileEncoding encoding)
    : mMap {std::move(map)},
      mNewEncoding {encoding}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void SetTileFormatEncoding::undo()
{
  auto& format = mMap->get_tile_format();

  format.set_encoding(mOldEncoding.value());
  format.set_compression(mOldCompression.value());

  mOldEncoding.reset();
  mOldCompression.reset();
}

void SetTileFormatEncoding::redo()
{
  auto& format = mMap->get_tile_format();

  mOldEncoding = format.encoding();
  mOldCompression = format.compression();

  format.set_encoding(mNewEncoding);
}

auto SetTileFormatEncoding::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_tile_format_encoding;
}

}  // namespace tactile
