#include "save_as_dialog.hpp"

#include <utility>  // move

#include <portable-file-dialogs.h>

#include "editor/events/save_events.hpp"
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
        cen::log::info("No suffix in specified file path, assuming preferred format");
        if (const auto& format = Prefs::GetPreferredFormat(); format == "YAML") {
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

    show = false;
  }
}

void OpenSaveAsDialog()
{
  show = true;
}

}  // namespace Tactile
