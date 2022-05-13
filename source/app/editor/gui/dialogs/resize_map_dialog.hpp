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

#include "editor/gui/dialogs/dialog.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Used to change the dimensions of a tilemap.
 */
class ResizeMapDialog final : public ADialog
{
 public:
  TACTILE_DEFAULT_COPY(ResizeMapDialog);
  TACTILE_DEFAULT_MOVE(ResizeMapDialog);

  ResizeMapDialog();

  ~ResizeMapDialog() override = default;

  void show(usize nCurrentRows, usize nCurrentColumns);

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

 private:
  usize mRows{};
  usize mColumns{};
};

}  // namespace tactile
