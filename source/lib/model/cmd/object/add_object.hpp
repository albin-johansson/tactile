// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/layer/object_type.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, MapDocument)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, Object)

namespace tactile::cmd {

class AddObject final : public Command {
 public:
  AddObject(MapDocument* document,
            const UUID& layer_id,
            ObjectType type,
            const Float2& pos,
            const Float2& size = {});

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  MapDocument* mDocument {};
  UUID mLayerId {};
  ObjectType mObjectType {};
  Float2 mPos {};
  Float2 mSize {};
  Maybe<UUID> mObjectId;
  Shared<Object> mObject;  /// The added object (needed to make subsequent commands work)
};

}  // namespace tactile::cmd
