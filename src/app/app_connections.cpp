#include "app_connections.hpp"

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
    : m_model{app->model_ptr()},
      m_window{app->window_ptr()}
{
  using win = window;
  using mod = model;

  // clang-format off

  connect(m_model, &mod::redraw,                m_window, &win::handle_draw);
  connect(m_model, &mod::enable_stamp_preview,  m_window, &win::enable_stamp_preview);
  connect(m_model, &mod::disable_stamp_preview, m_window, &win::disable_stamp_preview);
  connect(m_model, &mod::undo_state_updated,    m_window, &win::handle_undo_state_update);
  connect(m_model, &mod::redo_state_updated,    m_window, &win::handle_redo_state_update);
  connect(m_model, &mod::undo_text_updated,     m_window, &win::handle_undo_text_update);
  connect(m_model, &mod::redo_text_updated,     m_window, &win::handle_redo_text_update);
  connect(m_model, &mod::switched_map,          m_window, &win::switched_map);
  connect(m_model, &mod::added_tileset,         m_window, &win::handle_add_tileset);
  connect(m_model, &mod::removed_tileset,       m_window, &win::handle_removed_tileset);
  connect(m_model, &mod::added_layer,           m_window, &win::handle_added_layer);
  connect(m_model, &mod::removed_layer,         m_window, &win::handle_removed_layer);
  connect(m_model, &mod::selected_layer,        m_window, &win::handle_selected_layer);

  connect(m_window, &win::request_undo,               m_model, &mod::undo);
  connect(m_window, &win::request_redo,               m_model, &mod::redo);
  connect(m_window, &win::request_add_row,            m_model, &mod::add_row);
  connect(m_window, &win::request_add_col,            m_model, &mod::add_col);
  connect(m_window, &win::request_remove_row,         m_model, &mod::remove_row);
  connect(m_window, &win::request_remove_col,         m_model, &mod::remove_col);
  connect(m_window, &win::request_close_map,          m_model, &mod::close_map);
  connect(m_window, &win::request_select_map,         m_model, &mod::ui_selected_map);
  connect(m_window, &win::mouse_pressed,              m_model, &mod::mouse_pressed);
  connect(m_window, &win::mouse_moved,                m_model, &mod::mouse_moved);
  connect(m_window, &win::mouse_released,             m_model, &mod::mouse_released);
  connect(m_window, &win::mouse_entered,              m_model, &mod::mouse_entered);
  connect(m_window, &win::mouse_exited,               m_model, &mod::mouse_exited);
  connect(m_window, &win::request_increase_tile_size, m_model, &mod::increase_tile_size);
  connect(m_window, &win::request_decrease_tile_size, m_model, &mod::decrease_tile_size);
  connect(m_window, &win::request_reset_tile_size,    m_model, &mod::reset_tile_size);
  connect(m_window, &win::select_tool,                m_model, &mod::select_tool);
  connect(m_window, &win::ui_removed_tileset,         m_model, &mod::ui_removed_tileset);
  connect(m_window, &win::selected_tileset,           m_model, &mod::select_tileset);
  connect(m_window, &win::ui_requested_new_layer,     m_model, &mod::add_layer);
  connect(m_window, &win::ui_requested_remove_layer,  m_model, &mod::remove_layer);
  connect(m_window, &win::ui_selected_layer,          m_model, &mod::select_layer);

  connect(m_window, &win::request_resize_map,        app, &app::handle_resize_map);
  connect(m_window, &win::request_pan_up,            app, &app::handle_pan_up);
  connect(m_window, &win::request_pan_down,          app, &app::handle_pan_down);
  connect(m_window, &win::request_pan_right,         app, &app::handle_pan_right);
  connect(m_window, &win::request_pan_left,          app, &app::handle_pan_left);
  connect(m_window, &win::request_new_map,           app, &app::handle_new_map);
  connect(m_window, &win::request_new_tileset,       app, &app::handle_new_tileset);
  connect(m_window, &win::tileset_selection_changed, app, &app::tileset_selection_changed);
  connect(m_window, &win::save_as,                   app, &app::save_as);
  connect(m_window, &win::open_map,                  app, &app::open_map);

  // clang-format on
}

void app_connections::from_model()
{}

void app_connections::from_window()
{}

}  // namespace tactile
