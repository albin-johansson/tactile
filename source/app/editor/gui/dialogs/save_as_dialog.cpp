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

#include "save_as_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "editor/events/misc_events.hpp"
#include "io/file_dialog.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/logging.hpp"

namespace tactile {

void show_save_as_dialog(entt::dispatcher& dispatcher)
{
  auto dialog = FileDialog::save_map();
  if (dialog.is_okay()) {
    auto path = dialog.path();

    const auto ext = path.extension();
    const auto hasValidExtension = ext == ".yaml" || ext == ".yml" || ext == ".json" ||
                                   ext == ".tmj" || ext == ".tmx" || ext == ".xml";

    // TODO is this logic still required with new file dialogs?
    if (!hasValidExtension) {
      const auto& format = get_preferences().preferred_format();
      log_warning("Invalid file extension '{}', assuming '{}'", ext, format);

      if (format == "YAML") {
        path += ".yaml";
      }
      else if (format == "JSON") {
        path += ".json";
      }
      else if (format == "XML") {
        path += ".tmx";
      }
      else {
        log_error("Could not amend requested file path with valid extension!");
        return;
      }
    }

    dispatcher.enqueue<SaveAsEvent>(std::move(path));
  }
}

}  // namespace tactile
