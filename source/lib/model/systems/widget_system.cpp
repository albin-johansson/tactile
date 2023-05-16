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

#include "widget_system.hpp"

#include <utility>  // move

#include "common/type/maybe.hpp"

namespace tactile::sys {
namespace {

void _add_widget(Model& model,
                 const uint32 weight,
                 WidgetCallback callback,
                 Maybe<IsWidgetEnabledFn> is_enabled_fn)
{
  const auto widget_entity = model.create_entity();

  auto& widget = model.add<Widget>(widget_entity);
  widget.weight = weight;
  widget.callback = std::move(callback);

  if (is_enabled_fn.has_value()) {
    widget.is_enabled = std::move(*is_enabled_fn);
  }
}

}  // namespace

void sort_widgets(Model& model)
{
  auto sorter = [](const Widget& a, const Widget& b) { return a.weight < b.weight; };
  model.sort<Widget>(sorter);
}

void add_widget(Model& model, const uint32 weight, WidgetCallback callback)
{
  _add_widget(model, weight, std::move(callback), nothing);
}

void add_widget(Model& model,
                const uint32 weight,
                WidgetCallback callback,
                IsWidgetEnabledFn is_enabled_fn)
{
  _add_widget(model, weight, std::move(callback), std::move(is_enabled_fn));
}

void render_widgets(const Model& model, Dispatcher& dispatcher)
{
  for (const auto& [widget_entity, widget]: model.each<Widget>()) {
    if (!widget.is_enabled || widget.is_enabled(model)) {
      widget.callback(model, widget_entity, dispatcher);
    }
  }
}

}  // namespace tactile::sys
