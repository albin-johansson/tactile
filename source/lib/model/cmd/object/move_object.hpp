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

#include "core/layer/object.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile::cmd {

/// Command for moving an object in a map.
class MoveObject final : public Command {
 public:
  MoveObject(Shared<Object> object, const Float2& previous, const Float2& updated);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Shared<Object> mObject;
  Float2 mPreviousPos {};
  Float2 mUpdatedPos {};
};

}  // namespace tactile::cmd
