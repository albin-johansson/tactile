/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
