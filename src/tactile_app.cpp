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
  m_window = std::make_unique<gui::Window>();
  init_connections();

  m_window->show();
}

tactile_app::~tactile_app() noexcept = default;

void tactile_app::init_connections()
{
  using window = gui::Window;
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

  winToMod(&window::S_Undo,                &model::undo);
  winToMod(&window::S_Redo,                &model::redo);
  winToMod(&window::S_AddRow,              &model::add_row);
  winToMod(&window::S_AddCol,              &model::add_col);
  winToMod(&window::S_RemoveRow,           &model::remove_row);
  winToMod(&window::S_RemoveCol,           &model::remove_col);
  winToMod(&window::S_AboutToCloseMap,     &model::close_map);
  winToMod(&window::S_SelectMap,           &model::select_map);
  winToMod(&window::S_ZoomIn,              &model::increase_tile_size);
  winToMod(&window::S_ZoomOut,             &model::decrease_tile_size);
  winToMod(&window::S_ResetZoom,           &model::reset_tile_size);
  winToMod(&window::S_SelectedTool,        &model::select_tool);
  winToMod(&window::S_RemoveTileset,       &model::remove_tileset);
  winToMod(&window::S_SelectTileset,       &model::select_tileset);
  winToMod(&window::S_RenameTileset,       &model::set_tileset_name);
  winToMod(&window::S_SetTilesetSelection, &model::set_tileset_selection);

  winToMod(&window::S_MousePressed,  &model::mouse_pressed);
  winToMod(&window::S_MouseMoved,    &model::mouse_moved);
  winToMod(&window::S_MouseReleased, &model::mouse_released);
  winToMod(&window::S_MouseEntered,  &model::mouse_entered);
  winToMod(&window::S_MouseExited,   &model::mouse_exited);

  modToWin(&model::redraw,                   &window::ForceRedraw);
  modToWin(&model::enable_stamp_preview,     &window::EnableStampPreview);
  modToWin(&model::disable_stamp_preview,    &window::DisableStampPreview);
  modToWin(&model::undo_state_updated,       &window::OnUndoStateUpdated);
  modToWin(&model::redo_state_updated,       &window::OnRedoStateUpdated);
  modToWin(&model::undo_text_updated,        &window::OnUndoTextUpdated);
  modToWin(&model::redo_text_updated,        &window::OnRedoTextUpdated);
  modToWin(&model::clean_changed,            &window::OnCleanChanged);
  modToWin(&model::switched_map,             &window::OnSwitchedMap);
  modToWin(&model::added_tileset,            &window::OnAddedTileset);
  modToWin(&model::removed_tileset,          &window::OnRemovedTileset);
  modToWin(&model::renamed_tileset,          &window::OnRenamedTileset);
  modToWin(&model::added_layer,              &window::OnAddedLayer);
  modToWin(&model::added_duplicated_layer,   &window::OnAddedDuplicatedLayer);
  modToWin(&model::removed_layer,            &window::OnRemovedLayer);
  modToWin(&model::selected_layer,           &window::OnSelectedLayer);
  modToWin(&model::added_property,           &window::OnAddedProperty);
  modToWin(&model::about_to_remove_property, &window::OnAboutToRemoveProperty);
  modToWin(&model::updated_property,         &window::OnUpdatedProperty);
  modToWin(&model::renamed_property,         &window::OnRenamedProperty);
  modToWin(&model::changed_property_type,    &window::OnChangedPropertyType);
  modToWin(&model::show_map_properties,      &window::ShowMapProperties);
  modToWin(&model::show_layer_properties,    &window::ShowLayerProperties);

  fromWindow(&window::S_AddTileset, &tactile_app::handle_new_tileset);
  fromWindow(&window::S_ResizeMap,  &tactile_app::handle_resize_map);
  fromWindow(&window::S_PanUp,      &tactile_app::handle_pan_up);
  fromWindow(&window::S_PanDown,    &tactile_app::handle_pan_down);
  fromWindow(&window::S_PanRight,   &tactile_app::handle_pan_right);
  fromWindow(&window::S_PanLeft,    &tactile_app::handle_pan_left);
  fromWindow(&window::S_NewMap,     &tactile_app::handle_new_map);
  fromWindow(&window::S_Save,       &tactile_app::save);
  fromWindow(&window::S_SaveAs,     &tactile_app::save_as);
  fromWindow(&window::S_OpenMap,    &tactile_app::open_map);
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
    gui::resize_dialog::spawn([this](const row_t rows, const col_t cols) {
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
  m_window->OnNewMapAdded(m_model->get_document(id), id);
}

}  // namespace tactile
