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

#include "common/macros.hpp"
#include "common/primitives.hpp"
#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "common/type/func.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Model)

/// Callback with signature common for all widget functions.
/// The entity is the widget entity.
using WidgetCallback = Func<void(const Model&, Entity, Dispatcher&)>;

using IsWidgetEnabledFn = Func<bool(const Model&)>;

struct Widget final {
  uint32 weight {};         ///< Used when sorting widgets.
  WidgetCallback callback;  ///< Callback for the widget ImGui code.
  IsWidgetEnabledFn is_enabled;
};

}  // namespace tactile
