#include "save_as_dialog.hpp"

#include <utility>  // move

#include "editor/events/save_events.hpp"
#include "io/file_dialog.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace tactile {

void OpenSaveAsDialog(entt::dispatcher& dispatcher)
{
  auto dialog = FileDialog::SaveMap();
  if (dialog.IsOkay()) {
    auto path = dialog.GetPath();

    const auto ext = path.extension();
    const auto hasValidExtension = ext == ".yaml" || ext == ".yml" || ext == ".json" ||
                                   ext == ".tmx" || ext == ".xml";

    // TODO is this logic still required with new file dialogs?
    if (!hasValidExtension) {
      const auto& format = prefs::GetPreferredFormat();
      LogWarning("Invalid file extension '{}', assuming '{}'", ext, format);

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
        LogError("Could not amend requested file path with valid extension!");
        return;
      }
    }

    dispatcher.enqueue<SaveAsEvent>(std::move(path));
  }
}

}  // namespace tactile
