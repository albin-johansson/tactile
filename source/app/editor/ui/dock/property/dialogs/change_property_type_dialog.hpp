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

#include "core/attribute.hpp"
#include "core/common/vocabulary.hpp"
#include "core/type/maybe.hpp"
#include "core/type/string.hpp"
#include "core/uuid.hpp"
#include "editor/ui/dialog/dialog.hpp"

namespace tactile::ui {

class ChangePropertyTypeDialog final : public Dialog {
 public:
  TACTILE_DEFAULT_COPY(ChangePropertyTypeDialog);
  TACTILE_DEFAULT_MOVE(ChangePropertyTypeDialog);

  ChangePropertyTypeDialog();

  ~ChangePropertyTypeDialog() override = default;

  void show(const UUID& context_id, String name, AttributeType type);

 protected:
  void on_update(const DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_accept(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto is_current_input_valid(const DocumentModel& model) const
      -> bool override;

 private:
  Maybe<UUID> mContextId;
  AttributeType mCurrentType {AttributeType::String};
  Maybe<String> mPropertyName;
  Maybe<AttributeType> mPreviousType;
};

}  // namespace tactile::ui