// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Tileset)

namespace tactile::cmd {

class RenameTileset final : public Command {
 public:
  RenameTileset(Shared<Tileset> tileset, String name);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Tileset> mTileset;
  String mNewName;
  Maybe<String> mOldName;
};

}  // namespace tactile::cmd
