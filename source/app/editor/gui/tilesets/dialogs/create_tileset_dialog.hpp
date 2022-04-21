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

#include <array>       // array
#include <filesystem>  // path

#include "editor/gui/dialogs/dialog.hpp"

namespace tactile {

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
  std::filesystem::path mFullImagePath;
  std::array<char, 100> mPathPreviewBuffer{};
  int mTileWidth{32};
  int mTileHeight{32};

  void ShowImageFileDialog();
};

}  // namespace tactile
