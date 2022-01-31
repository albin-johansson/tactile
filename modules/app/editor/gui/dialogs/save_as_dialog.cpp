#include "save_as_dialog.hpp"

#include <utility>  // move

#include "editor/events/save_events.hpp"
#include "io/file_dialog.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace tactile {

void OpenSaveAsDialog(entt::dispatcher& dispatcher)
{
  auto dialog = file_dialog::save_map();
  if (dialog.is_okay()) {
    auto path = dialog.path();

    const auto ext = path.extension();
    const auto hasValidExtension = ext == ".yaml" || ext == ".yml" || ext == ".json" ||
                                   ext == ".tmx" || ext == ".xml";

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
