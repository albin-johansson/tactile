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

#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile {

class document_model;

class context_menu
{
 public:
  TACTILE_DEFAULT_COPY(context_menu)
  TACTILE_DEFAULT_MOVE(context_menu)

  explicit context_menu(const char* name);

  virtual ~context_menu() noexcept = default;

  void update(const document_model& model, entt::dispatcher& dispatcher);

  void show();

 protected:
  virtual void on_update([[maybe_unused]] const document_model& model,
                         [[maybe_unused]] entt::dispatcher& dispatcher)
  {}

 private:
  const char* mName{};
  bool mShow{};
};

}  // namespace tactile
