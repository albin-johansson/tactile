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
  connect(m_model, &model::redraw, m_window, &window::handle_draw);
  connect(m_model,
          &model::enable_stamp_preview,
          m_window,
          &window::enable_stamp_preview);
  connect(m_model,
          &model::disable_stamp_preview,
          m_window,
          &window::disable_stamp_preview);

  connect(m_model,
          &model::undo_state_updated,
          m_window,
          &window::handle_undo_state_update);
  connect(m_model,
          &model::redo_state_updated,
          m_window,
          &window::handle_redo_state_update);
  connect(m_model,
          &model::undo_text_updated,
          m_window,
          &window::handle_undo_text_update);
  connect(m_model,
          &model::redo_text_updated,
          m_window,
          &window::handle_redo_text_update);

  connect(m_window, &window::request_undo, m_model, &model::undo);
  connect(m_window, &window::request_redo, m_model, &model::redo);
  connect(m_window, &window::request_add_row, m_model, &model::add_row);
  connect(m_window, &window::request_add_col, m_model, &model::add_col);
  connect(m_window, &window::request_remove_row, m_model, &model::remove_row);
  connect(m_window, &window::request_remove_col, m_model, &model::remove_col);

  connect(m_window, &window::request_resize_map, app, &app::handle_resize_map);

  connect(m_window, &window::request_pan_up, app, &app::handle_pan_up);
  connect(m_window, &window::request_pan_down, app, &app::handle_pan_down);
  connect(m_window, &window::request_pan_right, app, &app::handle_pan_right);
  connect(m_window, &window::request_pan_left, app, &app::handle_pan_left);

  connect(m_window, &window::request_new_map, app, &app::handle_new_map);

  connect(m_window, &window::request_close_map, m_model, &model::close_map);
  connect(
      m_window, &window::request_select_map, m_model, &model::ui_selected_map);
  connect(m_window, &window::mouse_pressed, m_model, &model::mouse_pressed);
  connect(m_window, &window::mouse_moved, m_model, &model::mouse_moved);
  connect(m_window, &window::mouse_released, m_model, &model::mouse_released);
  connect(m_window, &window::mouse_entered, m_model, &model::mouse_entered);
  connect(m_window, &window::mouse_exited, m_model, &model::mouse_exited);

  connect(m_model, &model::switched_map, m_window, &window::switched_map);

  connect(m_window,
          &window::request_increase_tile_size,
          m_model,
          &model::increase_tile_size);
  connect(m_window,
          &window::request_decrease_tile_size,
          m_model,
          &model::decrease_tile_size);
  connect(m_window,
          &window::request_reset_tile_size,
          m_model,
          &model::reset_tile_size);

  connect(m_window, &window::select_tool, m_model, &model::select_tool);

  connect(
      m_window, &window::request_new_tileset, app, &app::handle_new_tileset);

  connect(
      m_model, &model::added_tileset, m_window, &window::handle_add_tileset);
  connect(m_model,
          &model::removed_tileset,
          m_window,
          &window::handle_removed_tileset);

  connect(m_window,
          &window::ui_removed_tileset,
          m_model,
          &model::ui_removed_tileset);
  connect(m_window, &window::selected_tileset, m_model, &model::select_tileset);

  connect(m_window,
          &window::tileset_selection_changed,
          app,
          &app::tileset_selection_changed);

  connect(m_window, &window::save_as, app, &app::save_as);
  connect(m_window, &window::open_map, app, &app::open_map);

  connect(
      m_window, &window::ui_requested_new_layer, m_model, &model::add_layer);
  connect(m_window,
          &window::ui_requested_remove_layer,
          m_model,
          &model::remove_layer);
  connect(m_window, &window::ui_selected_layer, m_model, &model::select_layer);

  connect(m_model, &model::added_layer, m_window, &window::handle_added_layer);
  connect(
      m_model, &model::removed_layer, m_window, &window::handle_removed_layer);
  connect(m_model,
          &model::selected_layer,
          m_window,
          &window::handle_selected_layer);
}

void app_connections::from_model()
{}

void app_connections::from_window()
{}

}  // namespace tactile
