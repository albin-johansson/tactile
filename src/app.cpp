#include "app.hpp"

#include "model.hpp"
#include "open_map.hpp"
#include "resize_dialog.hpp"
#include "save_service.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

using core::col_t;
using core::row_t;

app::app(int argc, char** argv)
    : QApplication{argc, argv},
      m_model{new core::model{}}
{
  setup_app();
  m_window = std::make_unique<gui::window>();
  init_connections();
  m_window->show();
}

app::~app() noexcept
{
  delete m_model;
}

void app::init_connections()
{
  using win = gui::window;
  using mod = core::model;

  const auto model_to_window = [this](auto&& sender, auto&& receiver) {
    connect(m_model, sender, m_window.get(), receiver);
  };

  const auto window_to_model = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, m_model, receiver);
  };

  const auto from_window = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, this, receiver);
  };

  // clang-format off

  model_to_window(&mod::redraw,                &win::force_redraw);
  model_to_window(&mod::enable_stamp_preview,  &win::enable_stamp_preview);
  model_to_window(&mod::disable_stamp_preview, &win::disable_stamp_preview);
  model_to_window(&mod::undo_state_updated,    &win::undo_state_updated);
  model_to_window(&mod::redo_state_updated,    &win::redo_state_updated);
  model_to_window(&mod::undo_text_updated,     &win::undo_text_updated);
  model_to_window(&mod::redo_text_updated,     &win::redo_text_updated);
  model_to_window(&mod::switched_map,          &win::switched_map);
  model_to_window(&mod::added_tileset,         &win::added_tileset);
  model_to_window(&mod::removed_tileset,       &win::removed_tileset);
  model_to_window(&mod::added_layer,           &win::added_layer);
  model_to_window(&mod::removed_layer,         &win::removed_layer);
  model_to_window(&mod::selected_layer,        &win::selected_layer);

  window_to_model(&win::ui_undo,                   &mod::undo);
  window_to_model(&win::ui_redo,                   &mod::redo);
  window_to_model(&win::ui_add_row,                &mod::add_row);
  window_to_model(&win::ui_add_col,                &mod::add_col);
  window_to_model(&win::ui_remove_row,             &mod::remove_row);
  window_to_model(&win::ui_remove_col,             &mod::remove_col);
  window_to_model(&win::ui_close_map,              &mod::close_map);
  window_to_model(&win::ui_select_map,             &mod::ui_selected_map);
  window_to_model(&win::ui_increase_zoom,          &mod::increase_tile_size);
  window_to_model(&win::ui_decrease_zoom,          &mod::decrease_tile_size);
  window_to_model(&win::ui_reset_tile_size,        &mod::reset_tile_size);
  window_to_model(&win::ui_selected_tool,          &mod::select_tool);
  window_to_model(&win::ui_removed_tileset,        &mod::ui_removed_tileset);
  window_to_model(&win::ui_selected_tileset,       &mod::select_tileset);
  window_to_model(&win::ui_requested_new_layer,    &mod::add_layer);
  window_to_model(&win::ui_requested_remove_layer, &mod::remove_active_layer);
  window_to_model(&win::ui_selected_layer,         &mod::select_layer);
  window_to_model(&win::ui_set_layer_visibility,   &mod::set_layer_visibility);
  window_to_model(&win::mouse_pressed,             &mod::mouse_pressed);
  window_to_model(&win::mouse_moved,               &mod::mouse_moved);
  window_to_model(&win::mouse_released,            &mod::mouse_released);
  window_to_model(&win::mouse_entered,             &mod::mouse_entered);
  window_to_model(&win::mouse_exited,              &mod::mouse_exited);

  from_window(&win::ui_resize_map,                &app::handle_resize_map);
  from_window(&win::ui_pan_up,                    &app::handle_pan_up);
  from_window(&win::ui_pan_down,                  &app::handle_pan_down);
  from_window(&win::ui_pan_right,                 &app::handle_pan_right);
  from_window(&win::ui_pan_left,                  &app::handle_pan_left);
  from_window(&win::ui_new_map,                   &app::handle_new_map);
  from_window(&win::ui_new_tileset,               &app::handle_new_tileset);
  from_window(&win::ui_tileset_selection_changed, &app::tileset_selection_changed);
  from_window(&win::ui_save_as,                   &app::save_as);
  from_window(&win::ui_open_map,                  &app::open_map);

  // clang-format on
}

void app::save_as(const QString& path)
{
  if (const auto* document = m_model->current_document()) {
    service::save(path, *document);
  }
}

void app::open_map(const QString& path)
{
  auto* document = service::open_map(path);

  const auto mapId = m_model->add_map(document);
  m_window->handle_new_map(document, mapId);

  document->each_tileset(
      [&](tileset_id tilesetId, const core::tileset& tileset) {
        m_window->added_tileset(mapId, tilesetId, tileset);
      });
}

void app::handle_resize_map()
{
  if (m_model->has_active_map()) {
    gui::resize_dialog::spawn(
        [this](row_t rows, col_t cols) { m_model->resize_map(rows, cols); });
  }
}

void app::handle_pan_up()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(0, document->current_tile_size());
  }
}

void app::handle_pan_down()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(0, -document->current_tile_size());
  }
}

void app::handle_pan_right()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(-document->current_tile_size(), 0);
  }
}

void app::handle_pan_left()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(document->current_tile_size(), 0);
  }
}

void app::handle_new_tileset()
{
  gui::tileset_dialog::spawn([this](const QImage& image,
                                    const QString& path,
                                    tile_width tileWidth,
                                    tile_height tileHeight,
                                    const QString& name) {
    m_model->ui_added_tileset(image, path, name, tileWidth, tileHeight);
  });
}

void app::tileset_selection_changed(core::position topLeft,
                                    core::position bottomRight)
{
  m_model->update_tileset_selection(topLeft, bottomRight);
}

void app::handle_new_map()
{
  const auto id = m_model->add_map();
  m_window->handle_new_map(m_model->get_document(id), id);
}

}  // namespace tactile
