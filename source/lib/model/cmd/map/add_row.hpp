// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/map.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"

namespace tactile::cmd {

class AddRow final : public Command {
 public:
  explicit AddRow(Shared<Map> map);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  usize mRowCount {1};
};

}  // namespace tactile::cmd
