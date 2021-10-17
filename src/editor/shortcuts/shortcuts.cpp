#include "shortcuts.hpp"

#include <memory>  // unique_ptr
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

}  // namespace

void LoadDefaultShortcuts()
{
  // File
  shortcuts.push_back(std::make_unique<NewMapShortcut>());
  shortcuts.push_back(std::make_unique<OpenMapShortcut>());
  shortcuts.push_back(std::make_unique<OpenSettingsShortcut>());
  shortcuts.push_back(std::make_unique<SaveShortcut>());
  shortcuts.push_back(std::make_unique<SaveAsShortcut>());

  // Edit
  shortcuts.push_back(std::make_unique<UndoShortcut>());
  shortcuts.push_back(std::make_unique<RedoShortcut>());

  shortcuts.push_back(std::make_unique<EnableStampShortcut>());
  shortcuts.push_back(std::make_unique<EnableEraserShortcut>());
  shortcuts.push_back(std::make_unique<EnableBucketShortcut>());

  shortcuts.push_back(std::make_unique<AddRowShortcut>());
  shortcuts.push_back(std::make_unique<AddColumnShortcut>());
  shortcuts.push_back(std::make_unique<RemoveRowShortcut>());
  shortcuts.push_back(std::make_unique<RemoveColumnShortcut>());

  shortcuts.push_back(std::make_unique<AddTilesetShortcut>());

  // View
  shortcuts.push_back(std::make_unique<CenterViewportShortcut>());
  shortcuts.push_back(std::make_unique<IncreaseViewportZoomShortcut>());
  shortcuts.push_back(std::make_unique<DecreaseViewportZoomShortcut>());

  shortcuts.push_back(std::make_unique<ToggleGridShortcut>());
  shortcuts.push_back(std::make_unique<ToggleUiShortcut>());

  shortcuts.push_back(std::make_unique<PanRightShortcut>());
  shortcuts.push_back(std::make_unique<PanLeftShortcut>());
  shortcuts.push_back(std::make_unique<PanUpShortcut>());
  shortcuts.push_back(std::make_unique<PanDownShortcut>());
}

void UpdateShortcuts(const Model& model,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  for (const auto& shortcut : shortcuts) {
    shortcut->Poll(model, event, dispatcher);
  }
}

}  // namespace Tactile
