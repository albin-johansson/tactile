// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_tileset.hpp"

#include <utility>  // move

#include "core/tile/tileset.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RenameTileset::RenameTileset(Shared<Tileset> tileset, String name)
    : mTileset {std::move(tileset)},
      mNewName {std::move(name)}
{
  if (!mTileset) {
    throw Exception {"Invalid null tileset!"};
  }
}

void RenameTileset::undo()
{
  mTileset->get_ctx().set_name(mOldName.value());
  mOldName.reset();
}

void RenameTileset::redo()
{
  mOldName = mTileset->get_ctx().name();
  mTileset->get_ctx().set_name(mNewName);
}

auto RenameTileset::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RenameTileset*>(cmd)) {
    if (mTileset->get_uuid() == other->mTileset->get_uuid()) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto RenameTileset::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_tileset;
}

}  // namespace tactile::cmd
