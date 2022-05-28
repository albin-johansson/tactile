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

#include "core/common/identifiers.hpp"
#include "core/common/maybe.hpp"
#include "editor/ui/dialogs/string_input_dialog.hpp"

namespace tactile {

/**
 * \brief Used to change the name of an existing layer.
 * \ingroup gui
 */
class RenameLayerDialog final : public AStringInputDialog
{
 public:
  RenameLayerDialog();

  void show(LayerID id, std::string oldName);

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto validate(const DocumentModel& model, std::string_view input) const
      -> bool override;

 private:
  Maybe<LayerID> mTargetId;
  Maybe<std::string> mOldName;
};

}  // namespace tactile
