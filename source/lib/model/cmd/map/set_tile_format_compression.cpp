// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_tile_format_compression.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

SetTileFormatCompression::SetTileFormatCompression(Shared<Map> map,
                                                   const CompressionType compression)
  : mMap {std::move(map)},
    mNewCompression {compression}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void SetTileFormatCompression::undo()
{
  auto& format = mMap->get_tile_format();
  format.set_compression(mOldCompression.value());

  mOldCompression.reset();
}

void SetTileFormatCompression::redo()
{
  auto& format = mMap->get_tile_format();

  mOldCompression = format.compression();
  format.set_compression(mNewCompression);
}

auto SetTileFormatCompression::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_tile_format_compression;
}

}  // namespace tactile
