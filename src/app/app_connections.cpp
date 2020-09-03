#include "app_connections.hpp"

#include <qobject.h>

#include "core_model.hpp"
#include "window.hpp"

namespace tactile {

using gui::window;
using model::core_model;

namespace {

template <typename Sender, typename Signal, typename Receiver, typename Slot>
void connect(Sender&& sender, Signal&& signal, Receiver&& receiver, Slot&& slot)
{
  QObject::connect(sender, signal, receiver, slot);
}

}  // namespace

app_connections::app_connections(app& app)
    : m_core{app.core_ptr()}, m_window{app.window_ptr()}
{
  connect(
      m_core, &core_model::redraw_requested, m_window, &window::handle_draw);

  init_command_connections(app);
  init_camera_connections(app);
  init_tilemap_connections(app);

  connect(m_window,
          &window::request_increase_tile_size,
          m_core,
          &core_model::handle_increase_tile_size);

  connect(m_window,
          &window::request_decrease_tile_size,
          m_core,
          &core_model::handle_decrease_tile_size);

  connect(m_window,
          &window::request_reset_tile_size,
          m_core,
          &core_model::handle_reset_tile_size);

  connect(
      m_window, &window::request_new_tileset, &app, &app::handle_new_tileset);
}

void app_connections::init_command_connections(app& app) noexcept
{
  connect(m_core,
          &core_model::undo_state_updated,
          m_window,
          &window::handle_undo_state_update);

  connect(m_core,
          &core_model::redo_state_updated,
          m_window,
          &window::handle_redo_state_update);

  connect(m_core,
          &core_model::undo_text_updated,
          m_window,
          &window::handle_undo_text_update);

  connect(m_core,
          &core_model::redo_text_updated,
          m_window,
          &window::handle_redo_text_update);

  connect(m_window, &window::request_undo, m_core, &core_model::undo);
  connect(m_window, &window::request_redo, m_core, &core_model::redo);

  connect(m_window, &window::request_add_row, m_core, &core_model::add_row);
  connect(m_window, &window::request_add_col, m_core, &core_model::add_col);
  connect(
      m_window, &window::request_remove_row, m_core, &core_model::remove_row);
  connect(
      m_window, &window::request_remove_col, m_core, &core_model::remove_col);

  connect(m_window, &window::request_resize_map, &app, &app::handle_resize_map);
}

void app_connections::init_camera_connections(app& app) noexcept
{
  connect(m_window, &window::request_pan_up, &app, &app::handle_pan_up);
  connect(m_window, &window::request_pan_down, &app, &app::handle_pan_down);
  connect(m_window, &window::request_pan_right, &app, &app::handle_pan_right);
  connect(m_window, &window::request_pan_left, &app, &app::handle_pan_left);
}

void app_connections::init_tilemap_connections(app& app) noexcept
{
  connect(m_window, &window::request_new_map, &app, &app::handle_new_map);

  connect(m_window,
          &window::request_close_map,
          m_core,
          &core_model::handle_close_map);

  connect(
      m_window, &window::request_select_map, m_core, &core_model::select_map);
}

}  // namespace tactile
