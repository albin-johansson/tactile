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
  using model = core::model;

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

  winToMod(&win::ui_undo,                  &model::undo);
  winToMod(&win::ui_redo,                  &model::redo);
  winToMod(&win::ui_add_row,               &model::add_row);
  winToMod(&win::ui_add_col,               &model::add_col);
  winToMod(&win::ui_remove_row,            &model::remove_row);
  winToMod(&win::ui_remove_col,            &model::remove_col);
  winToMod(&win::ui_about_to_close_map,    &model::close_map);
  winToMod(&win::ui_select_map,            &model::select_map);
  winToMod(&win::ui_increase_zoom,         &model::increase_tile_size);
  winToMod(&win::ui_decrease_zoom,         &model::decrease_tile_size);
  winToMod(&win::ui_reset_tile_size,       &model::reset_tile_size);
  winToMod(&win::ui_selected_tool,         &model::select_tool);
  winToMod(&win::ui_remove_tileset,        &model::remove_tileset);
  winToMod(&win::ui_select_tileset,        &model::select_tileset);
  winToMod(&win::ui_rename_tileset,        &model::set_tileset_name);
  winToMod(&win::ui_set_tileset_selection, &model::set_tileset_selection);

  winToMod(&win::mouse_pressed,            &model::mouse_pressed);
  winToMod(&win::mouse_moved,              &model::mouse_moved);
  winToMod(&win::mouse_released,           &model::mouse_released);
  winToMod(&win::mouse_entered,            &model::mouse_entered);
  winToMod(&win::mouse_exited,             &model::mouse_exited);

  modToWin(&model::redraw,                   &win::force_redraw);
  modToWin(&model::enable_stamp_preview,     &win::enable_stamp_preview);
  modToWin(&model::disable_stamp_preview,    &win::disable_stamp_preview);
  modToWin(&model::undo_state_updated,       &win::undo_state_updated);
  modToWin(&model::redo_state_updated,       &win::redo_state_updated);
  modToWin(&model::undo_text_updated,        &win::undo_text_updated);
  modToWin(&model::redo_text_updated,        &win::redo_text_updated);
  modToWin(&model::clean_changed,            &win::clean_changed);
  modToWin(&model::switched_map,             &win::switched_map);
  modToWin(&model::added_tileset,            &win::added_tileset);
  modToWin(&model::removed_tileset,          &win::removed_tileset);
  modToWin(&model::added_layer,              &win::added_layer);
  modToWin(&model::added_duplicated_layer,   &win::added_duplicated_layer);
  modToWin(&model::removed_layer,            &win::removed_layer);
  modToWin(&model::selected_layer,           &win::selected_layer);
  modToWin(&model::added_property,           &win::added_property);
  modToWin(&model::about_to_remove_property, &win::about_to_remove_property);
  modToWin(&model::updated_property,         &win::updated_property);
  modToWin(&model::renamed_property,         &win::renamed_property);

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
  parse::parse_error error;
  if (auto* document = service::open_map(path, &error)) {
    const auto id = m_model->add_map(document);
    m_window->when_new_map_added(document, id, QFileInfo{path}.baseName());
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
  m_window->when_new_map_added(m_model->get_document(id), id);
}

}  // namespace tactile
