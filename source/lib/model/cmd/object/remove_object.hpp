// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {
class MapDocument;
class Object;
}  // namespace tactile

namespace tactile::cmd {

class RemoveObject final : public Command {
 public:
  RemoveObject(MapDocument* document, const UUID& layer_id, const UUID& object_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  MapDocument* mDocument {};
  UUID mLayerId {};
  Shared<Object> mObject;
};

}  // namespace tactile::cmd
