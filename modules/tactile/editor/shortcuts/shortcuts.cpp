#include "shortcuts.hpp"

#include <memory>  // unique_ptr, make_unique
#include <vector>  // vector

#include "edit_shortcuts.hpp"
#include "file_shortcuts.hpp"
#include "view_shortcuts.hpp"

namespace tactile {
namespace {

inline std::vector<std::unique_ptr<AShortcut>> gShortcuts;

template <typename T>
void LoadShortcut()
{
  gShortcuts.push_back(std::make_unique<T>());
}

}  // namespace

void LoadDefaultShortcuts()
{
  // File
  LoadShortcut<NewMapShortcut>();
  LoadShortcut<OpenMapShortcut>();
  LoadShortcut<OpenSettingsShortcut>();
  LoadShortcut<SaveShortcut>();
  LoadShortcut<SaveAsShortcut>();

  // Edit
  LoadShortcut<UndoShortcut>();
  LoadShortcut<RedoShortcut>();

  LoadShortcut<EnableStampShortcut>();
  LoadShortcut<EnableEraserShortcut>();
  LoadShortcut<EnableBucketShortcut>();
  LoadShortcut<EnableObjectSelectionShortcut>();

  LoadShortcut<AddRowShortcut>();
  LoadShortcut<AddColumnShortcut>();
  LoadShortcut<RemoveRowShortcut>();
  LoadShortcut<RemoveColumnShortcut>();

  LoadShortcut<AddTilesetShortcut>();

  // View
  LoadShortcut<CenterViewportShortcut>();
  LoadShortcut<IncreaseViewportZoomShortcut>();
  LoadShortcut<DecreaseViewportZoomShortcut>();

  LoadShortcut<ToggleGridShortcut>();
  LoadShortcut<ToggleUiShortcut>();

  LoadShortcut<PanRightShortcut>();
  LoadShortcut<PanLeftShortcut>();
  LoadShortcut<PanUpShortcut>();
  LoadShortcut<PanDownShortcut>();
}

void UpdateShortcuts(const Model& model,
                     const WidgetManager& widgets,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  for (const auto& shortcut : gShortcuts) {
    shortcut->Poll(model, widgets, event, dispatcher);
  }
}

}  // namespace tactile
