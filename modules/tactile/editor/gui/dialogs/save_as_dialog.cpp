#include "save_as_dialog.hpp"

#include <utility>  // move

#include <portable-file-dialogs.h>
#include <tactile_io.hpp>

#include "editor/events/save_events.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"

namespace Tactile {
namespace {

constinit bool gShow = false;

}  // namespace

void UpdateSaveAsDialog(entt::dispatcher& dispatcher)
{
  if (gShow) {
    auto path =
        pfd::save_file{"Save as...", "", {"Map File", "*.json *.tmx *.xml *.yml *.yaml"}}
            .result();

    if (!path.empty()) {
      if (!path.ends_with(".json") && !path.ends_with(".tmx") &&
          !path.ends_with(".xml") && !path.ends_with(".yml") &&
          !path.ends_with(".yaml")) {
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
