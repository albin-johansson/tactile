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

#include "common/enum/tool_type.hpp"
#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "common/type/func.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/maybe.hpp"
#include "core/viewport.hpp"
#include "model/model.hpp"

namespace tactile {

using ToolFn = Func<void(Model&, Entity, Dispatcher&)>;
using ToolMouseInfoFn = Func<void(Model&, Entity, const ViewportMouseInfo&, Dispatcher&)>;
using ModelPredicate = Func<bool(const Model&)>;  // TODO move elsewhere

/// Context component that tracks tool entities.
struct ToolContext final {
  HashMap<ToolType, Entity> tools;  ///< All loaded tools.
  Maybe<ToolType> active_tool;      ///< The currently active tool, if any.
};

/// Component that describes a mouse tool.
/// \note It's fine to leave callbacks unbound, only set the callbacks used by the tool.
struct Tool final {
  ToolFn on_activated;          ///< Called when the tool has been activated (selected).
  ToolFn on_deactivated;        ///< Called when the tool has been deactivated.
  ToolFn on_entered;            ///< Called when the mouse enters the viewport.
  ToolFn on_exited;             ///< Called when the mouse exits the viewport.
  ToolMouseInfoFn on_pressed;   ///< Called when the mouse is pressed.
  ToolMouseInfoFn on_dragged;   ///< Called when the mouse is dragged.
  ToolMouseInfoFn on_released;  ///< Called when the mouse is released.
  ModelPredicate is_available;  ///< Determines whether the tool is available.
};

}  // namespace tactile
