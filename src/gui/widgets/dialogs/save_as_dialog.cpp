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
        pfd::save_file{"Save as...", "", {"Map File", "*.json *.tmx *.xml *.yml *.yaml"}}
            .result();

    if (!path.empty()) {
      if (!path.ends_with(".json") && !path.ends_with(".tmx") &&
          !path.ends_with(".xml") && !path.ends_with(".yml") && !path.ends_with(".yaml"))
      {
        CENTURION_LOG_INFO("No suffix in requested file path, using preferred format...");
        path += (Prefs::GetPreferredFormat() == "JSON") ? ".json" : ".tmx";
      }

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
