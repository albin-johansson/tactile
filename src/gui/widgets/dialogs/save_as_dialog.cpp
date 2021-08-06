#include "save_as_dialog.hpp"

#include "events/save_as_event.hpp"
#include "gui/widgets/common/file_dialog.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

inline bool show = false;

}  // namespace

void UpdateSaveAsDialog(entt::dispatcher& dispatcher)
{
  if (show)
  {
    const auto filter =
        (Prefs::GetPreferredFormat() == "JSON") ? ".json,.tmx" : ".tmx,.json";
    const auto res = FileDialogExport("SaveAsDialog", "Save as...", filter);

    if (res == FileDialogResult::Success)
    {
      dispatcher.enqueue<SaveAsEvent>(GetFileDialogSelectedPath());
      show = false;
    }
    else if (res == FileDialogResult::Close)
    {
      show = false;
    }
  }
}

void OpenSaveAsDialog()
{
  show = true;
}

}  // namespace Tactile
