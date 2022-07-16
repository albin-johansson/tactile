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

#include <entt/fwd.hpp>

#include "core/common/macros.hpp"

namespace tactile {
class DocumentModel;
}  // namespace tactile

namespace tactile::ui {

class ContextMenu
{
 public:
  TACTILE_DEFAULT_COPY(ContextMenu);
  TACTILE_DEFAULT_MOVE(ContextMenu);

  explicit ContextMenu(const char* name);

  virtual ~ContextMenu() noexcept = default;

  void update(const DocumentModel& model, entt::dispatcher& dispatcher);

  void show();

 protected:
  virtual void on_update([[maybe_unused]] const DocumentModel& model,
                         [[maybe_unused]] entt::dispatcher&    dispatcher)
  {}

 private:
  const char* mName {};
  bool        mShow {};
};

}  // namespace tactile::ui
