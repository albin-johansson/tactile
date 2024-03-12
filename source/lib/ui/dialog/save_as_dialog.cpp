// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

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
