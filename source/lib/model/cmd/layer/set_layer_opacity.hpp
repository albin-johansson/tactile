// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/layer.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile::cmd {

class SetLayerOpacity final : public Command {
 public:
  SetLayerOpacity(Shared<Layer> layer, float opacity);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Layer> mLayer;
  float mNewOpacity {};
  Maybe<float> mOldOpacity;
};

}  // namespace tactile::cmd
