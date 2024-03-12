// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(MapDocument)
TACTILE_FWD_DECLARE_CLASS(Layer)

namespace cmd {

class DuplicateLayer final : public Command {
 public:
  DuplicateLayer(MapDocument* document, const UUID& layer_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  MapDocument* mDocument {};
  UUID mLayerId {};
  Shared<Layer> mNewLayer;
  Maybe<usize> mNewIndex;
};

}  // namespace cmd
}  // namespace tactile
