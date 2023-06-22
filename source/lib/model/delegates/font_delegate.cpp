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

#include "font_delegate.hpp"

#include <algorithm>  // min, max

#include "model/persistence/settings.hpp"
#include "ui/constants.hpp"

namespace tactile {

void on_reset_font_size(Model& model, Dispatcher& dispatcher, const ResetFontSizeEvent&)
{
  auto& settings = model.get<Settings>();
  settings.set_font_size(kDefFontSize);

  dispatcher.trigger(ReloadFontsEvent {});
}

void on_increase_font_size(Model& model,
                           Dispatcher& dispatcher,
                           const IncreaseFontSizeEvent&)
{
  auto& settings = model.get<Settings>();

  const auto new_size = std::min(settings.get_font_size() + 2, kMaxFontSize);
  settings.set_font_size(new_size);

  dispatcher.trigger(ReloadFontsEvent {});
}

void on_decrease_font_size(Model& model,
                           Dispatcher& dispatcher,
                           const DecreaseFontSizeEvent&)
{
  auto& settings = model.get<Settings>();

  const auto new_size = std::max(settings.get_font_size() - 2, kMinFontSize);
  settings.set_font_size(new_size);

  dispatcher.trigger(ReloadFontsEvent {});
}

}  // namespace tactile
