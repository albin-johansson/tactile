#include "tactile_app.hpp"

#include "model.hpp"
#include "open_map.hpp"
#include "open_map_error_dialog.hpp"
#include "parse_error.hpp"
#include "resize_dialog.hpp"
#include "save_service.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

tactile_app::tactile_app(int argc, char** argv)
    : QApplication{argc, argv}
    , m_model{new core::model{}}
{
  setup_app();
  m_window = std::make_unique<gui::window>();
  init_connections();
  m_window->show();
}

tactile_app::~tactile_app() noexcept
{
  delete m_model;
}

void tactile_app::init_connections()
{
  using win = gui::window;
  using mod = core::model;

  const auto modToWin = [this](auto&& sender, auto&& receiver) {
    connect(m_model, sender, m_window.get(), receiver);
  };

  const auto winToMod = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, m_model, receiver);
  };

  const auto fromWindow = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, this, receiver);
  };

  // clang-format off

  winToMod(&win::ui_undo,                  &mod::undo);
  winToMod(&win::ui_redo,                  &mod::redo);
  winToMod(&win::ui_add_row,               &mod::add_row);
  winToMod(&win::ui_add_col,               &mod::add_col);
  winToMod(&win::ui_remove_row,            &mod::remove_row);
  winToMod(&win::ui_remove_col,            &mod::remove_col);
  winToMod(&win::ui_close_map,             &mod::close_map);
  winToMod(&win::ui_select_map,            &mod::select_map);
  winToMod(&win::ui_increase_zoom,         &mod::increase_tile_size);
  winToMod(&win::ui_decrease_zoom,         &mod::decrease_tile_size);
  winToMod(&win::ui_reset_tile_size,       &mod::reset_tile_size);
  winToMod(&win::ui_selected_tool,         &mod::select_tool);
  winToMod(&win::ui_remove_tileset,        &mod::remove_tileset);
  winToMod(&win::ui_select_tileset,        &mod::select_tileset);
  winToMod(&win::ui_rename_tileset,        &mod::set_tileset_name);
  winToMod(&win::ui_set_tileset_selection, &mod::set_tileset_selection);

  winToMod(&win::mouse_pressed,            &mod::mouse_pressed);
  winToMod(&win::mouse_moved,              &mod::mouse_moved);
  winToMod(&win::mouse_released,           &mod::mouse_released);
  winToMod(&win::mouse_entered,            &mod::mouse_entered);
  winToMod(&win::mouse_exited,             &mod::mouse_exited);

  modToWin(&mod::redraw,                 &win::force_redraw);
  modToWin(&mod::enable_stamp_preview,   &win::enable_stamp_preview);
  modToWin(&mod::disable_stamp_preview,  &win::disable_stamp_preview);
  modToWin(&mod::undo_state_updated,     &win::undo_state_updated);
  modToWin(&mod::redo_state_updated,     &win::redo_state_updated);
  modToWin(&mod::undo_text_updated,      &win::undo_text_updated);
  modToWin(&mod::redo_text_updated,      &win::redo_text_updated);
  modToWin(&mod::clean_changed,          &win::clean_changed);
  modToWin(&mod::switched_map,           &win::switched_map);
  modToWin(&mod::added_tileset,          &win::added_tileset);
  modToWin(&mod::removed_tileset,        &win::removed_tileset);
  modToWin(&mod::added_layer,            &win::added_layer);
  modToWin(&mod::added_duplicated_layer, &win::added_duplicated_layer);
  modToWin(&mod::removed_layer,          &win::removed_layer);
  modToWin(&mod::selected_layer,         &win::selected_layer);
  modToWin(&mod::added_property,         &win::added_property);
  modToWin(&mod::removed_property,       &win::removed_property);

  fromWindow(&win::ui_add_tileset, &tactile_app::handle_new_tileset);
  fromWindow(&win::ui_resize_map,  &tactile_app::handle_resize_map);
  fromWindow(&win::ui_pan_up,      &tactile_app::handle_pan_up);
  fromWindow(&win::ui_pan_down,    &tactile_app::handle_pan_down);
  fromWindow(&win::ui_pan_right,   &tactile_app::handle_pan_right);
  fromWindow(&win::ui_pan_left,    &tactile_app::handle_pan_left);
  fromWindow(&win::ui_new_map,     &tactile_app::handle_new_map);
  fromWindow(&win::ui_save,        &tactile_app::save);
  fromWindow(&win::ui_save_as,     &tactile_app::save_as);
  fromWindow(&win::ui_open_map,    &tactile_app::open_map);

  // clang-format on
}

void tactile_app::save()
{
  if (auto* document = m_model->current_document()) {
    if (!document->path().exists()) {  // Documents don't have a path initially
      m_window->trigger_save_as();
    } else {
      service::save(document->path().absoluteFilePath(), *document);
      document->mark_as_clean();
    }
  }
}

void tactile_app::save_as(const QString& path)
{
  if (auto* document = m_model->current_document()) {
    service::save(path, *document);
    const QFileInfo file{path};
    document->mark_as_clean();
    document->set_path(file);
    m_window->set_active_tab_name(file.baseName());
  }
}

void tactile_app::open_map(const QString& path)
{
  tmx::parse_error error;
  if (auto* document = service::open_map(path, &error)) {
    const auto mapId = m_model->add_map(document);
    m_window->handle_new_map(document, mapId, QFileInfo{path}.baseName());
    document->each_tileset(
        [&](const tileset_id id, const core::tileset& tileset) {
          m_window->added_tileset(mapId, id, tileset);
        });
  } else {
    gui::open_map_error_dialog dialog{m_window.get()};
    dialog.set_file(path);
    dialog.set_error_message(to_message(error));
    dialog.exec();
  }
}

void tactile_app::handle_resize_map()
{
  if (m_model->has_active_map()) {
    gui::resize_dialog::spawn([this](const row_t rows, const col_t cols) {
      m_model->resize_map(rows, cols);
    });
  }
}

void tactile_app::handle_pan_up()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(0, document->current_tile_size());
  }
}

void tactile_app::handle_pan_down()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(0, -document->current_tile_size());
  }
}

void tactile_app::handle_pan_right()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(-document->current_tile_size(), 0);
  }
}

void tactile_app::handle_pan_left()
{
  if (const auto* document = m_model->current_document()) {
    m_window->handle_move_camera(document->current_tile_size(), 0);
  }
}

void tactile_app::handle_new_tileset()
{
  gui::tileset_dialog::spawn([this](const gui::tileset_info& info) {
    m_model->create_tileset(info.image,
                            info.path,
                            info.name,
                            info.tileWidth,
                            info.tileHeight);
  });
}

void tactile_app::handle_new_map()
{
  const auto id = m_model->add_map();
  m_window->handle_new_map(m_model->get_document(id), id);
}

}  // namespace tactile
