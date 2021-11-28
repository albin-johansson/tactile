#include "shortcuts.hpp"

#include <memory>  // unique_ptr, make_unique
#include <vector>  // vector

#include "editor/gui/widgets/menus/edit_menu.hpp"
#include "editor/shortcuts/edit/add_column_shortcut.hpp"
#include "editor/shortcuts/edit/add_row_shortcut.hpp"
#include "editor/shortcuts/edit/add_tileset_shortcut.hpp"
#include "editor/shortcuts/edit/enable_bucket_tool.hpp"
#include "editor/shortcuts/edit/enable_eraser_shortcut.hpp"
#include "editor/shortcuts/edit/enable_stamp_shortcut.hpp"
#include "editor/shortcuts/edit/redo_shortcut.hpp"
#include "editor/shortcuts/edit/remove_column_shortcut.hpp"
#include "editor/shortcuts/edit/remove_row_shortcut.hpp"
#include "editor/shortcuts/edit/undo_shortcut.hpp"
#include "editor/shortcuts/file/new_map_shortcut.hpp"
#include "editor/shortcuts/file/open_map_shortcut.hpp"
#include "editor/shortcuts/file/open_settings_shortcut.hpp"
#include "editor/shortcuts/file/save_as_shortcut.hpp"
#include "editor/shortcuts/file/save_shortcut.hpp"
#include "editor/shortcuts/view/center_viewport_shortcut.hpp"
#include "editor/shortcuts/view/decrease_viewport_zoom_shortcut.hpp"
#include "editor/shortcuts/view/increase_viewport_zoom_shortcut.hpp"
#include "editor/shortcuts/view/pan_down_shortcut.hpp"
#include "editor/shortcuts/view/pan_left_shortcut.hpp"
#include "editor/shortcuts/view/pan_right_shortcut.hpp"
#include "editor/shortcuts/view/pan_up_shortcut.hpp"
#include "editor/shortcuts/view/toggle_grid_shortcut.hpp"
#include "editor/shortcuts/view/toggle_ui_shortcut.hpp"

namespace Tactile {
namespace {

inline std::vector<std::unique_ptr<AShortcut>> shortcuts;

template <typename T>
void LoadShortcut()
{
  shortcuts.push_back(std::make_unique<T>());
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
  for (const auto& shortcut : shortcuts) {
    shortcut->Poll(model, event, dispatcher);
  }
}

}  // namespace Tactile
