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

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "io/file_dialog.hpp"
#include "io/save_formats.hpp"
#include "model/events/file_events.hpp"
#include "model/services/settings/settings.hpp"

namespace tactile {

void show_save_as_dialog(ModelView model)
{
  auto dialog = FileDialog::save_map();
  if (dialog.is_okay()) {
    auto path = dialog.path();
    const auto has_valid_extension = has_supported_tactile_yaml_extension(path) ||
                                     has_supported_tiled_json_extension(path) ||
                                     has_supported_tiled_xml_extension(path);

    if (!has_valid_extension) {
      const auto& settings = model.get_settings();
      const auto& format = settings.get_preferred_format();

      spdlog::warn("[UI] Invalid file extension {}, assuming {} format",
                   path.extension(),
                   get_human_readable_name(format));

      path += get_file_extension(format);
    }

    model.enqueue<SaveAsEvent>(std::move(path));
  }
}

}  // namespace tactile
