#include "shortcuts.hpp"

#include <memory>  // unique_ptr, make_unique
#include <vector>  // vector

#include "edit_shortcuts.hpp"
#include "editor/gui/widgets/menus/edit_menu.hpp"
#include "file_shortcuts.hpp"
#include "view_shortcuts.hpp"

namespace Tactile {
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
                     const SDL_KeyboardEvent& event,
                     entt::dispatcher& dispatcher)
{
  for (const auto& shortcut : gShortcuts) {
    shortcut->Poll(model, event, dispatcher);
  }
}

}  // namespace Tactile
