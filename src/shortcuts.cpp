#include "shortcuts.hpp"

#include "core/events/maps/add_column_event.hpp"
#include "core/events/maps/add_map_event.hpp"
#include "core/events/maps/add_row_event.hpp"
#include "core/events/maps/remove_column_event.hpp"
#include "core/events/maps/remove_row_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/tools/select_tool_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/events/viewport/center_viewport_event.hpp"
#include "core/events/viewport/decrease_viewport_zoom_event.hpp"
#include "core/events/viewport/increase_viewport_zoom_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/menus/edit_menu.hpp"
#include "gui/widgets/menus/file_menu.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

void OnCtrlAltKeyStroke(const cen::scan_code key)
{
  if (key == cen::scancodes::s)
  {
    EnableSettingsDialog();
  }
}

void OnAltShiftKeyStroke(const cen::scan_code key,
                         const Model& model,
                         entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (!document)
  {
    return;
  }

  if (key == cen::scancodes::r && document->GetRowCount() != 1_row)
  {
    dispatcher.enqueue<RemoveRowEvent>();
  }
  else if (key == cen::scancodes::c && document->GetColumnCount() != 1_col)
  {
    dispatcher.enqueue<RemoveColumnEvent>();
  }
}

void OnCtrlKeyStroke(const cen::scan_code key,
                     const Model& model,
                     entt::dispatcher& dispatcher)
{
  const auto* document = model.GetActiveDocument();
  if (key == cen::scancodes::n)
  {
    dispatcher.enqueue<AddMapEvent>();
  }
  else if (key == cen::scancodes::g)
  {
    Prefs::SetShowGrid(!Prefs::GetShowGrid());
  }
  else if (key == cen::scancodes::o)
  {
    EnableOpenMapDialog();
  }
  else if (key == cen::scancodes::t && document)
  {
    EnableTilesetDialog();
  }
  else if (key == cen::scancodes::space && document)
  {
    dispatcher.enqueue<CenterViewportEvent>();
  }
  else if (key == cen::scancodes::z && document && document->CanUndo())
  {
    dispatcher.enqueue<UndoEvent>();
  }
  else if (key == cen::scancodes::y && document && document->CanRedo())
  {
    dispatcher.enqueue<RedoEvent>();
  }
  else if (key == cen::scan_code{SDLK_PLUS} && document)
  {
    dispatcher.enqueue<IncreaseViewportZoomEvent>();
  }
  else if (key == cen::scan_code{SDLK_MINUS} && document &&
           document->CanDecreaseViewportTileSize())
  {
    dispatcher.enqueue<DecreaseViewportZoomEvent>();
  }
}

void OnAltKeyStroke(const cen::scan_code key, entt::dispatcher& dispatcher)
{
  if (key == cen::scancodes::r)
  {
    dispatcher.enqueue<AddRowEvent>();
  }
  else if (key == cen::scancodes::c)
  {
    dispatcher.enqueue<AddColumnEvent>();
  }
}

void OnPlainKeyStroke(const cen::scan_code key, entt::dispatcher& dispatcher)
{
  if (key == cen::scancodes::s)
  {
    dispatcher.enqueue<SelectToolEvent>(MouseToolType::Stamp);
  }
  else if (key == cen::scancodes::b)
  {
    dispatcher.enqueue<SelectToolEvent>(MouseToolType::Bucket);
  }
  else if (key == cen::scancodes::e)
  {
    dispatcher.enqueue<SelectToolEvent>(MouseToolType::Eraser);
  }
}

}  // namespace

void UpdateShortcuts(const Model& model,
                     const cen::keyboard_event& event,
                     entt::dispatcher& dispatcher)
{
  if (!event.pressed())
  {
    return;
  }

  const auto ctrl = event.is_active(cen::key_modifier::ctrl);
  const auto shift = event.is_active(cen::key_modifier::shift);
  const auto alt = event.is_active(cen::key_modifier::alt);

  if (ctrl && alt)
  {
    OnCtrlAltKeyStroke(event.scan());
  }
  else if (alt && shift)
  {
    OnAltShiftKeyStroke(event.scan(), model, dispatcher);
  }
  else if (ctrl)
  {
    OnCtrlKeyStroke(event.scan(), model, dispatcher);
  }
  else if (alt)
  {
    OnAltKeyStroke(event.scan(), dispatcher);
  }
  else
  {
    OnPlainKeyStroke(event.scan(), dispatcher);
  }
}

}  // namespace Tactile
