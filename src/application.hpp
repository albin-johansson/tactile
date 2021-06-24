#pragma once

#include <centurion.hpp>  // window, gl_context
#include <entt.hpp>       // dispatcher

#include "aliases/unique.hpp"
#include "core/events/add_map_event.hpp"
#include "core/events/center_viewport_event.hpp"
#include "core/events/open_map_event.hpp"
#include "core/events/quit_event.hpp"
#include "core/events/redo_event.hpp"
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

  void OnCtrlKeyStroke(cen::scan_code key);

  void OnCtrlAltKeyStroke(cen::scan_code key);

  void OnAddMapEvent(const AddMapEvent& event);

  void OnOpenMapEvent(const OpenMapEvent& event);

  void OnUndoEvent(const UndoEvent& event);

  void OnRedoEvent(const RedoEvent& event);

  void OnCenterViewportEvent(const CenterViewportEvent& event);

  void OnQuitEvent(const QuitEvent& event);
};

}  // namespace tactile
