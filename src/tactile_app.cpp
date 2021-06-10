#include "tactile_app.hpp"

#include "model.hpp"
#include "open_map_document.hpp"
#include "open_map_error_dialog.hpp"
#include "parse_error.hpp"
#include "resize_dialog.hpp"
#include "save_map_document.hpp"
#include "setup_app.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

tactile_app::tactile_app(int argc, char** argv)
    : QApplication{argc, argv}
    , m_model{std::make_unique<core::model>()}
{
  setup_app();

  // It is necessary to create the window after calling the setup_app-function
  m_window = std::make_unique<Window>();
  init_connections();

  m_window->show();
}

tactile_app::~tactile_app() noexcept = default;

void tactile_app::init_connections()
{
  using model = core::model;

  const auto modToWin = [this](auto&& sender, auto&& receiver) {
    connect(m_model.get(), sender, m_window.get(), receiver);
  };

  const auto winToMod = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, m_model.get(), receiver);
  };

  const auto fromWindow = [this](auto&& sender, auto&& receiver) {
    connect(m_window.get(), sender, this, receiver);
  };

  // clang-format off

  winToMod(&Window::S_Undo,                &model::undo);
  winToMod(&Window::S_Redo,                &model::redo);
  winToMod(&Window::S_AddRow,              &model::add_row);
  winToMod(&Window::S_AddCol,              &model::add_col);
  winToMod(&Window::S_RemoveRow,           &model::remove_row);
  winToMod(&Window::S_RemoveCol,           &model::remove_col);
  winToMod(&Window::S_AboutToCloseMap,     &model::close_map);
  winToMod(&Window::S_SelectMap,           &model::select_map);
  winToMod(&Window::S_ZoomIn,              &model::increase_tile_size);
  winToMod(&Window::S_ZoomOut,             &model::decrease_tile_size);
  winToMod(&Window::S_ResetZoom,           &model::reset_tile_size);
  winToMod(&Window::S_SelectedTool,        &model::select_tool);
  winToMod(&Window::S_RemoveTileset,       &model::remove_tileset);
  winToMod(&Window::S_SelectTileset,       &model::select_tileset);
  winToMod(&Window::S_RenameTileset,       &model::set_tileset_name);
  winToMod(&Window::S_SetTilesetSelection, &model::set_tileset_selection);

  winToMod(&Window::S_MousePressed,  &model::mouse_pressed);
  winToMod(&Window::S_MouseMoved,    &model::mouse_moved);
  winToMod(&Window::S_MouseReleased, &model::mouse_released);
  winToMod(&Window::S_MouseEntered,  &model::mouse_entered);
  winToMod(&Window::S_MouseExited,   &model::mouse_exited);

  modToWin(&model::redraw,                   &Window::ForceRedraw);
  modToWin(&model::enable_stamp_preview,     &Window::EnableStampPreview);
  modToWin(&model::disable_stamp_preview,    &Window::DisableStampPreview);
  modToWin(&model::undo_state_updated,       &Window::OnUndoStateUpdated);
  modToWin(&model::redo_state_updated,       &Window::OnRedoStateUpdated);
  modToWin(&model::undo_text_updated,        &Window::OnUndoTextUpdated);
  modToWin(&model::redo_text_updated,        &Window::OnRedoTextUpdated);
  modToWin(&model::clean_changed,            &Window::OnCleanChanged);
  modToWin(&model::switched_map,             &Window::OnSwitchedMap);
  modToWin(&model::added_tileset,            &Window::OnAddedTileset);
  modToWin(&model::removed_tileset,          &Window::OnRemovedTileset);
  modToWin(&model::renamed_tileset,          &Window::OnRenamedTileset);
  modToWin(&model::added_layer,              &Window::OnAddedLayer);
  modToWin(&model::added_duplicated_layer,   &Window::OnAddedDuplicatedLayer);
  modToWin(&model::removed_layer,            &Window::OnRemovedLayer);
  modToWin(&model::selected_layer,           &Window::OnSelectedLayer);
  modToWin(&model::added_property,           &Window::OnAddedProperty);
  modToWin(&model::about_to_remove_property, &Window::OnAboutToRemoveProperty);
  modToWin(&model::updated_property,         &Window::OnUpdatedProperty);
  modToWin(&model::renamed_property,         &Window::OnRenamedProperty);
  modToWin(&model::changed_property_type,    &Window::OnChangedPropertyType);
  modToWin(&model::show_map_properties,      &Window::ShowMapProperties);
  modToWin(&model::show_layer_properties,    &Window::ShowLayerProperties);

  fromWindow(&Window::S_AddTileset, &tactile_app::handle_new_tileset);
  fromWindow(&Window::S_ResizeMap,  &tactile_app::handle_resize_map);
  fromWindow(&Window::S_PanUp,      &tactile_app::handle_pan_up);
  fromWindow(&Window::S_PanDown,    &tactile_app::handle_pan_down);
  fromWindow(&Window::S_PanRight,   &tactile_app::handle_pan_right);
  fromWindow(&Window::S_PanLeft,    &tactile_app::handle_pan_left);
  fromWindow(&Window::S_NewMap,     &tactile_app::handle_new_map);
  fromWindow(&Window::S_Save,       &tactile_app::save);
  fromWindow(&Window::S_SaveAs,     &tactile_app::save_as);
  fromWindow(&Window::S_OpenMap,    &tactile_app::open_map);
  // clang-format on
}

void tactile_app::save()
{
  if (auto* document = m_model->current_document())
  {
    if (document->has_path())
    {
      save_map_document(document->absolute_path(), *document);
      document->mark_as_clean();
    }
    else
    {
      m_window->TriggerSaveAs();
    }
  }
}

void tactile_app::save_as(const QString& path)
{
  if (auto* document = m_model->current_document())
  {
    save_map_document(path, *document);

    const QFileInfo file{path};
    document->mark_as_clean();
    document->set_path(file);

    m_window->SetActiveTabName(file.baseName());
  }
}

void tactile_app::open_map(const QString& path)
{
  parse::parse_error error;
  if (auto* document = open_map_document(path, error))
  {
    const auto id = m_model->add_map(document);
    m_window->OnNewMapAdded(document, id, QFileInfo{path}.baseName());
  }
  else
  {
    OpenMapErrorDialog dialog{m_window.get()};
    dialog.SetFile(QFileInfo{path});
    dialog.SetErrorMessage(to_message(error));
    dialog.exec();
  }
}

void tactile_app::handle_resize_map()
{
  if (m_model->has_active_map())
  {
    ResizeDialog::Spawn([this](const row_t rows, const col_t cols) {
      m_model->resize_map(rows, cols);
    });
  }
}

void tactile_app::handle_pan_up()
{
  if (const auto* document = m_model->current_document())
  {
    m_window->MoveViewport(0, document->current_tile_size());
  }
}

void tactile_app::handle_pan_down()
{
  if (const auto* document = m_model->current_document())
  {
    m_window->MoveViewport(0, -document->current_tile_size());
  }
}

void tactile_app::handle_pan_right()
{
  if (const auto* document = m_model->current_document())
  {
    m_window->MoveViewport(-document->current_tile_size(), 0);
  }
}

void tactile_app::handle_pan_left()
{
  if (const auto* document = m_model->current_document())
  {
    m_window->MoveViewport(document->current_tile_size(), 0);
  }
}

void tactile_app::handle_new_tileset()
{
  TilesetDialog::Spawn([this](const TilesetInfo& info) {
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
  m_window->OnNewMapAdded(m_model->get_document(id), id);
}

}  // namespace tactile
