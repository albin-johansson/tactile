#include "save_as_dialog.hpp"

#include <portable-file-dialogs.h>

#include <utility>  // move

#include "events/save_events.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constinit bool show = false;

}  // namespace

void UpdateSaveAsDialog(entt::dispatcher& dispatcher)
{
  if (show) {
    auto path =
        pfd::save_file{"Save as...", "", {"JSON File", "*.json", "TMX File", "*.tmx"}}
            .result();

    if (!path.ends_with(".json") && !path.ends_with(".tmx")) {
      CENTURION_LOG_INFO("No suffix in requested file path, using preferred format...");
      path += (Prefs::GetPreferredFormat() == "JSON") ? ".json" : ".tmx";
    }

    if (!path.empty()) {
      dispatcher.enqueue<SaveAsEvent>(std::move(path));
    }

    show = false;
  }
}

void OpenSaveAsDialog()
{
  show = true;
}

}  // namespace Tactile
