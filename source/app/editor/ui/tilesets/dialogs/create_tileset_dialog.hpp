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

#include <array>  // array

#include "core/common/filesystem.hpp"
#include "core/common/math.hpp"
#include "editor/ui/dialogs/dialog.hpp"

namespace tactile::ui {

class CreateTilesetDialog final : public ADialog
{
 public:
  CreateTilesetDialog();

  void open();

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_current_input_valid(const DocumentModel& model) const
      -> bool override;

 private:
  fs::path              mFullImagePath;
  std::array<char, 100> mPathPreviewBuffer {};
  Vector2i              mTileSize {32, 32};

  void show_image_file_dialog();
};

}  // namespace tactile::ui
