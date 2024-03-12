// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_zstd_compression_level.hpp"

#include <utility>  // move

#include "core/layer/tile_format.hpp"
#include "core/map.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

SetZstdCompressionLevel::SetZstdCompressionLevel(Shared<Map> map, const int level)
    : mMap {std::move(map)},
      mNewLevel {level}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void SetZstdCompressionLevel::undo()
{
  auto& format = mMap->get_tile_format();

  format.set_zstd_compression_level(mOldLevel.value());
  mOldLevel.reset();
}

void SetZstdCompressionLevel::redo()
{
  auto& format = mMap->get_tile_format();

  mOldLevel = format.zstd_compression_level();
  format.set_zstd_compression_level(mNewLevel);
}

auto SetZstdCompressionLevel::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetZstdCompressionLevel*>(cmd)) {
    mNewLevel = other->mNewLevel;
    return true;
  }

  return false;
}

auto SetZstdCompressionLevel::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_zstd_compression_level;
}

}  // namespace tactile::cmd
