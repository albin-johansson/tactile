#include "app_connections.hpp"

#include <qobject.h>

#include "model.hpp"
#include "window.hpp"

namespace tactile {

using core::model;
using gui::window;

namespace {

template <typename Sender, typename Signal, typename Receiver, typename Slot>
void connect(Sender&& sender, Signal&& signal, Receiver&& receiver, Slot&& slot)
{
  QObject::connect(sender, signal, receiver, slot);
}

}  // namespace

app_connections::app_connections(app* app)
    : m_core{app->model_ptr()},
      m_window{app->window_ptr()}
{
  connect(m_core, &model::redraw, m_window, &window::handle_draw);
  connect(m_core,
          &model::enable_stamp_preview,
          m_window,
          &window::enable_stamp_preview);
  connect(m_core,
          &model::disable_stamp_preview,
          m_window,
          &window::disable_stamp_preview);

  init_command_connections(app);
  init_camera_connections(app);
  init_map_connections(app);

  connect(m_window,
          &window::request_increase_tile_size,
          m_core,
          &model::increase_tile_size);

  connect(m_window,
          &window::request_decrease_tile_size,
          m_core,
          &model::decrease_tile_size);

  connect(m_window,
          &window::request_reset_tile_size,
          m_core,
          &model::reset_tile_size);

  connect(m_window, &window::select_tool, m_core, &model::select_tool);

  connect(
      m_window, &window::request_new_tileset, app, &app::handle_new_tileset);

  connect(m_window, &window::removed_tileset, m_core, &model::remove_tileset);

  connect(m_window, &window::selected_tileset, m_core, &model::select_tileset);

  connect(m_window,
          &window::tileset_selection_changed,
          app,
          &app::tileset_selection_changed);

  connect(m_window, &window::save_as, app, &app::save_as);
  connect(m_window, &window::open_map, app, &app::open_map);
}

void app_connections::init_command_connections(app* app) noexcept
{
  connect(m_core,
          &model::undo_state_updated,
          m_window,
          &window::handle_undo_state_update);

  connect(m_core,
          &model::redo_state_updated,
          m_window,
          &window::handle_redo_state_update);

  connect(m_core,
          &model::undo_text_updated,
          m_window,
          &window::handle_undo_text_update);

  connect(m_core,
          &model::redo_text_updated,
          m_window,
          &window::handle_redo_text_update);

  connect(m_window, &window::request_undo, m_core, &model::undo);
  connect(m_window, &window::request_redo, m_core, &model::redo);

  connect(m_window, &window::request_add_row, m_core, &model::add_row);
  connect(m_window, &window::request_add_col, m_core, &model::add_col);
  connect(m_window, &window::request_remove_row, m_core, &model::remove_row);
  connect(m_window, &window::request_remove_col, m_core, &model::remove_col);

  connect(m_window, &window::request_resize_map, app, &app::handle_resize_map);
}

void app_connections::init_camera_connections(app* app) noexcept
{
  connect(m_window, &window::request_pan_up, app, &app::handle_pan_up);
  connect(m_window, &window::request_pan_down, app, &app::handle_pan_down);
  connect(m_window, &window::request_pan_right, app, &app::handle_pan_right);
  connect(m_window, &window::request_pan_left, app, &app::handle_pan_left);
}

void app_connections::init_map_connections(app* app) noexcept
{
  connect(m_window, &window::request_new_map, app, &app::handle_new_map);

  connect(m_window, &window::request_close_map, m_core, &model::close_map);

  connect(m_window, &window::request_select_map, m_core, &model::select_map);

  connect(m_window, &window::mouse_pressed, m_core, &model::mouse_pressed);
  connect(m_window, &window::mouse_moved, m_core, &model::mouse_moved);
  connect(m_window, &window::mouse_released, m_core, &model::mouse_released);
  connect(m_window, &window::mouse_entered, m_core, &model::mouse_entered);
  connect(m_window, &window::mouse_exited, m_core, &model::mouse_exited);

  connect(m_core, &model::switched_map, m_window, &window::switched_map);
}

}  // namespace tactile
