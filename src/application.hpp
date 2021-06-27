#pragma once

#include <centurion.hpp>  // window, gl_context
#include <entt.hpp>       // dispatcher

#include "aliases/unique.hpp"
#include "core/events/add_column_event.hpp"
#include "core/events/add_map_event.hpp"
#include "core/events/add_row_event.hpp"
#include "core/events/add_tileset_event.hpp"
#include "core/events/center_viewport_event.hpp"
#include "core/events/open_map_event.hpp"
#include "core/events/quit_event.hpp"
#include "core/events/redo_event.hpp"
#include "core/events/remove_column_event.hpp"
#include "core/events/remove_row_event.hpp"
#include "core/events/select_layer_event.hpp"
#include "core/events/select_map_event.hpp"
#include "core/events/undo_event.hpp"
#include "core/model.hpp"

namespace tactile {

class Application final
{
 public:
  Application(cen::window&& window, cen::gl_context&& context);

  auto Run() -> int;

 private:
  cen::window mWindow;
  [[maybe_unused]] cen::gl_context mContext;
  entt::dispatcher mDispatcher;

  Unique<Model> mModel;
  bool mQuit{};

  void PollEvents();

  void UpdateFrame();

  void OnCtrlAltKeyStroke(cen::scan_code key);

  void OnAltShiftKeyStroke(cen::scan_code key);

  void OnCtrlKeyStroke(cen::scan_code key);

  void OnAltKeyStroke(cen::scan_code key);

  void OnAddMapEvent();

  void OnOpenMapEvent(const OpenMapEvent& event);

  void OnAddTilesetEvent(const AddTilesetEvent& event);

  void OnUndoEvent(const UndoEvent& event);

  void OnRedoEvent(const RedoEvent& event);

  void OnCenterViewportEvent();

  void OnSelectMapEvent(const SelectMapEvent& event);

  void OnSelectLayerEvent(const SelectLayerEvent& event);

  void OnAddRowEvent();

  void OnAddColumnEvent();

  void OnRemoveRowEvent();

  void OnRemoveColumnEvent();

  void OnQuitEvent();
};

}  // namespace tactile
