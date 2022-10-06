/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "core/cmd/command.hpp"
#include "core/layer/object_type.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/uuid.hpp"

namespace tactile {
class MapDocument;
}  // namespace tactile

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
};

}  // namespace tactile::cmd
