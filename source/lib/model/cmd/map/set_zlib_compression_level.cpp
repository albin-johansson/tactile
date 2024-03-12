// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_zlib_compression_level.hpp"

#include <utility>  // move

#include "core/layer/tile_format.hpp"
#include "core/map.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

SetZlibCompressionLevel::SetZlibCompressionLevel(Shared<Map> map, const int level)
    : mMap {std::move(map)},
      mNewLevel {level}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void SetZlibCompressionLevel::undo()
{
  auto& format = mMap->get_tile_format();

  format.set_zlib_compression_level(mOldLevel.value());
  mOldLevel.reset();
}

void SetZlibCompressionLevel::redo()
{
  auto& format = mMap->get_tile_format();

  mOldLevel = format.zlib_compression_level();
  format.set_zlib_compression_level(mNewLevel);
}

auto SetZlibCompressionLevel::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetZlibCompressionLevel*>(cmd)) {
    mNewLevel = other->mNewLevel;
    return true;
  }

  return false;
}

auto SetZlibCompressionLevel::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_zlib_compression_level;
}

}  // namespace tactile::cmd
