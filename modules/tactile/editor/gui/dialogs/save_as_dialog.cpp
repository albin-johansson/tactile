#include "save_as_dialog.hpp"

#include <utility>  // move

#include "editor/events/save_events.hpp"
#include "io/file_dialog.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace Tactile {
namespace {

constinit bool gShow = false;

}  // namespace

void UpdateSaveAsDialog(entt::dispatcher& dispatcher)
{
  if (gShow) {
    auto dialog = FileDialog::SaveMap();
    if (dialog.IsOkay()) {
      auto path = dialog.GetPath().string();

      // TODO is this logic still required with new file dialogs?
      if (!path.ends_with(".yaml") && !path.ends_with(".yml") &&
          !path.ends_with(".tmx") && !path.ends_with(".xml") &&
          !path.ends_with(".json")) {
        const auto& format = Prefs::GetPreferredFormat();
        LogInfo("No suffix provided in requested file path, using preferred format ({})",
                format);
        if (format == "YAML") {
          path += ".yaml";
        }
        else if (format == "JSON") {
          path += ".json";
        }
        else if (format == "XML") {
          path += ".tmx";
        }
      }

      dispatcher.enqueue<SaveAsEvent>(std::move(path));
    }

    gShow = false;
  }
}

void OpenSaveAsDialog()
{
  gShow = true;
}

}  // namespace Tactile
