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

#include "save_as_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "io/file_dialog.hpp"
#include "model/event/document_events.hpp"
#include "model/settings.hpp"
#include "tactile/core/io/save/save_format_id.hpp"

namespace tactile::ui {

void show_save_as_dialog(entt::dispatcher& dispatcher)
{
  auto dialog = FileDialog::save_map();
  if (dialog.is_okay()) {
    auto path = dialog.path();
    const auto has_valid_extension = has_valid_tactile_yaml_extension(path) ||
                                     has_valid_tiled_json_extension(path) ||
                                     has_valid_tiled_xml_extension(path);

    if (!has_valid_extension) {
      const auto& format = get_settings().get_preferred_format();
      spdlog::warn("Invalid file extension {}, assuming {}",
                   path.extension(),
                   to_string(format));
      path += get_file_extension(format);
    }

    dispatcher.enqueue<SaveAsEvent>(std::move(path));
  }
}

}  // namespace tactile::ui
