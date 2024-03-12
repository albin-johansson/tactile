// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/map.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile::cmd {

class SetLayerVisible final : public Command {
 public:
  SetLayerVisible(Shared<Map> map, const UUID& layer_id, bool visible);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Map> mMap;
  UUID mLayerId {};
  bool mNewVisibility {};
  Maybe<bool> mOldVisibility;
};

}  // namespace tactile::cmd
