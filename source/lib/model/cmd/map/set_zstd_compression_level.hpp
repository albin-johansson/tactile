// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile {
class Map;
}  // namespace tactile

namespace tactile::cmd {

class SetZstdCompressionLevel final : public Command {
 public:
  SetZstdCompressionLevel(Shared<Map> map, int level);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  int mNewLevel {};
  Maybe<int> mOldLevel;
};

}  // namespace tactile::cmd
