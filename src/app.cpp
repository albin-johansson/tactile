#include "app.hpp"

#include "model.hpp"
#include "open_map.hpp"
#include "resize_dialog.hpp"
#include "save_service.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

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

  const auto mod_to_win = [this](auto&& sender, auto&& receiver) {
    connect(m_model, sender, m_window.get(), receiver);
  };

  const auto win_to_mod = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, m_model, receiver);
  };

  const auto from_window = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, this, receiver);
  };

  // clang-format off

  mod_to_win(&mod::redraw,                 &win::force_redraw);
  mod_to_win(&mod::enable_stamp_preview,   &win::enable_stamp_preview);
  mod_to_win(&mod::disable_stamp_preview,  &win::disable_stamp_preview);
  mod_to_win(&mod::undo_state_updated,     &win::undo_state_updated);
  mod_to_win(&mod::redo_state_updated,     &win::redo_state_updated);
  mod_to_win(&mod::undo_text_updated,      &win::undo_text_updated);
  mod_to_win(&mod::redo_text_updated,      &win::redo_text_updated);
  mod_to_win(&mod::switched_map,           &win::switched_map);
  mod_to_win(&mod::added_tileset,          &win::added_tileset);
  mod_to_win(&mod::removed_tileset,        &win::removed_tileset);
  mod_to_win(&mod::added_layer,            &win::added_layer);
  mod_to_win(&mod::added_duplicated_layer, &win::added_duplicated_layer);
  mod_to_win(&mod::removed_layer,          &win::removed_layer);
  mod_to_win(&mod::selected_layer,         &win::selected_layer);
  mod_to_win(&mod::moved_layer_back,       &win::moved_layer_up);
  mod_to_win(&mod::moved_layer_forward,    &win::moved_layer_down);

  win_to_mod(&win::ui_undo,                 &mod::undo);
  win_to_mod(&win::ui_redo,                 &mod::redo);
  win_to_mod(&win::ui_add_row,              &mod::add_row);
  win_to_mod(&win::ui_add_col,              &mod::add_col);
  win_to_mod(&win::ui_remove_row,           &mod::remove_row);
  win_to_mod(&win::ui_remove_col,           &mod::remove_col);
  win_to_mod(&win::ui_close_map,            &mod::close_map);
  win_to_mod(&win::ui_select_map,           &mod::ui_selected_map);
  win_to_mod(&win::ui_increase_zoom,        &mod::increase_tile_size);
  win_to_mod(&win::ui_decrease_zoom,        &mod::decrease_tile_size);
  win_to_mod(&win::ui_reset_tile_size,      &mod::reset_tile_size);
  win_to_mod(&win::ui_selected_tool,        &mod::select_tool);
  win_to_mod(&win::ui_removed_tileset,      &mod::ui_removed_tileset);
  win_to_mod(&win::ui_selected_tileset,     &mod::select_tileset);
  win_to_mod(&win::ui_add_layer,            &mod::add_layer);
  win_to_mod(&win::ui_remove_layer,         &mod::remove_layer);
  win_to_mod(&win::ui_selected_layer,       &mod::select_layer);
  win_to_mod(&win::ui_set_layer_visibility, &mod::set_layer_visibility);
  win_to_mod(&win::ui_set_layer_opacity,    &mod::set_layer_opacity);
  win_to_mod(&win::ui_set_layer_name,       &mod::set_layer_name);
  win_to_mod(&win::ui_move_layer_down,      &mod::move_layer_back);
  win_to_mod(&win::ui_move_layer_up,        &mod::move_layer_forward);
  win_to_mod(&win::ui_duplicate_layer,      &mod::duplicate_layer);
  win_to_mod(&win::mouse_pressed,           &mod::mouse_pressed);
  win_to_mod(&win::mouse_moved,             &mod::mouse_moved);
  win_to_mod(&win::mouse_released,          &mod::mouse_released);
  win_to_mod(&win::mouse_entered,           &mod::mouse_entered);
  win_to_mod(&win::mouse_exited,            &mod::mouse_exited);

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
    gui::resize_dialog::spawn([this](row_t rows, col_t cols) {
      m_model->resize_map(rows, cols);
    });
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
  gui::tileset_dialog::spawn([this](const gui::tileset_info& info) {
    m_model->ui_added_tileset(info.image,
                              info.path,
                              info.name,
                              info.tileWidth,
                              info.tileHeight);
  });
}

void app::tileset_selection_changed(const core::tileset::selection& selection)
{
  m_model->set_tileset_selection(selection);
}

void app::handle_new_map()
{
  const auto id = m_model->add_map();
  m_window->handle_new_map(m_model->get_document(id), id);
}

}  // namespace tactile
