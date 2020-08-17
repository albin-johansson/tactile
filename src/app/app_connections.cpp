#include "app_connections.hpp"

#include <QObject>

#include "core_model.hpp"
#include "window.hpp"

namespace tactile {

using model::core_model;
using gui::window;

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
  connect(m_window, &window::request_redraw, m_core, &core_model::handle_draw);
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

  connect(m_window,
          &window::reload_stylesheet,
          &app,
          &app::handle_reload_stylesheet);
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

  connect(m_window, &window::request_undo, &app, &app::handle_undo);
  connect(m_window, &window::request_redo, &app, &app::handle_redo);

  connect(m_window, &window::request_add_row, &app, &app::handle_add_row);
  connect(m_window, &window::request_add_col, &app, &app::handle_add_col);
  connect(m_window, &window::request_remove_row, &app, &app::handle_remove_row);
  connect(m_window, &window::request_remove_col, &app, &app::handle_remove_col);

  connect(m_window, &window::request_resize_map, &app, &app::handle_resize_map);
}

void app_connections::init_camera_connections(app& app) noexcept
{
  connect(m_window, &window::request_pan_up, &app, &app::handle_pan_up);
  connect(m_window, &window::request_pan_down, &app, &app::handle_pan_down);
  connect(m_window, &window::request_pan_right, &app, &app::handle_pan_right);
  connect(m_window, &window::request_pan_left, &app, &app::handle_pan_left);

  connect(m_window,
          &window::request_center_camera,
          &app,
          &app::handle_center_camera);
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
