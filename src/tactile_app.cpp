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
    , m_model{std::make_unique<core::Model>()}
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
  using Model = core::Model;

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

  winToMod(&Window::S_Undo,                &Model::Undo);
  winToMod(&Window::S_Redo,                &Model::Redo);
  winToMod(&Window::S_AddRow,              &Model::AddRow);
  winToMod(&Window::S_AddCol,              &Model::AddColumn);
  winToMod(&Window::S_RemoveRow,           &Model::RemoveRow);
  winToMod(&Window::S_RemoveCol,           &Model::RemoveColumn);
  winToMod(&Window::S_AboutToCloseMap,     &Model::CloseMap);
  winToMod(&Window::S_SelectMap,           &Model::SelectMap);
  winToMod(&Window::S_ZoomIn,              &Model::IncreaseTileSize);
  winToMod(&Window::S_ZoomOut,             &Model::DecreaseTileSize);
  winToMod(&Window::S_ResetZoom,           &Model::ResetTileSize);
  winToMod(&Window::S_SelectedTool,        &Model::SelectTool);
  winToMod(&Window::S_RemoveTileset,       &Model::RemoveTileset);
  winToMod(&Window::S_SelectTileset,       &Model::SelectTileset);
  winToMod(&Window::S_RenameTileset,       &Model::SetTilesetName);
  winToMod(&Window::S_SetTilesetSelection, &Model::SetTilesetSelection);

  winToMod(&Window::S_MousePressed,  &Model::OnMousePressed);
  winToMod(&Window::S_MouseMoved,    &Model::OnMouseMoved);
  winToMod(&Window::S_MouseReleased, &Model::OnMouseReleased);
  winToMod(&Window::S_MouseEntered,  &Model::OnMouseEntered);
  winToMod(&Window::S_MouseExited,   &Model::OnMouseExited);

  modToWin(&Model::S_Redraw,                &Window::ForceRedraw);
  modToWin(&Model::S_EnableStampPreview,    &Window::OnEnableStampPreview);
  modToWin(&Model::S_DisableStampPreview,   &Window::OnDisableStampPreview);
  modToWin(&Model::S_UndoStateUpdated,      &Window::OnUndoStateUpdated);
  modToWin(&Model::S_RedoStateUpdated,      &Window::OnRedoStateUpdated);
  modToWin(&Model::S_UndoTextUpdated,       &Window::OnUndoTextUpdated);
  modToWin(&Model::S_RedoTextUpdated,       &Window::OnRedoTextUpdated);
  modToWin(&Model::S_CleanChanged,          &Window::OnCleanChanged);
  modToWin(&Model::S_SwitchedMap,           &Window::OnSwitchedMap);
  modToWin(&Model::S_AddedTileset,          &Window::OnAddedTileset);
  modToWin(&Model::S_RemovedTileset,        &Window::OnRemovedTileset);
  modToWin(&Model::S_RenamedTileset,        &Window::OnRenamedTileset);
  modToWin(&Model::S_AddedLayer,            &Window::OnAddedLayer);
  modToWin(&Model::S_AddedDuplicatedLayer,  &Window::OnAddedDuplicatedLayer);
  modToWin(&Model::S_RemovedLayer,          &Window::OnRemovedLayer);
  modToWin(&Model::S_SelectedLayer,         &Window::OnSelectedLayer);
  modToWin(&Model::S_AddedProperty,         &Window::OnAddedProperty);
  modToWin(&Model::S_AboutToRemoveProperty, &Window::OnAboutToRemoveProperty);
  modToWin(&Model::S_UpdatedProperty,       &Window::OnUpdatedProperty);
  modToWin(&Model::S_RenamedProperty,       &Window::OnRenamedProperty);
  modToWin(&Model::S_ChangedPropertyType,   &Window::OnChangedPropertyType);
  modToWin(&Model::S_ShowMapProperties,     &Window::OnShowMapProperties);
  modToWin(&Model::S_ShowLayerProperties,   &Window::OnShowLayerProperties);

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
  if (auto* document = m_model->CurrentDocument())
  {
    if (document->HasPath())
    {
      save_map_document(document->AbsolutePath(), *document);
      document->MarkAsClean();
    }
    else
    {
      m_window->TriggerSaveAs();
    }
  }
}

void tactile_app::save_as(const QString& path)
{
  if (auto* document = m_model->CurrentDocument())
  {
    save_map_document(path, *document);

    const QFileInfo file{path};
    document->MarkAsClean();
    document->SetPath(file);

    m_window->SetActiveTabName(file.baseName());
  }
}

void tactile_app::open_map(const QString& path)
{
  parse::parse_error error;
  if (auto* document = open_map_document(path, error))
  {
    const auto id = m_model->AddMap(document);
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
  if (m_model->HasActiveMap())
  {
    ResizeDialog::Spawn([this](const row_t rows, const col_t cols) {
      m_model->ResizeMap(rows, cols);
    });
  }
}

void tactile_app::handle_pan_up()
{
  if (const auto* document = m_model->CurrentDocument())
  {
    m_window->MoveViewport(0, document->CurrentTileSize());
  }
}

void tactile_app::handle_pan_down()
{
  if (const auto* document = m_model->CurrentDocument())
  {
    m_window->MoveViewport(0, -document->CurrentTileSize());
  }
}

void tactile_app::handle_pan_right()
{
  if (const auto* document = m_model->CurrentDocument())
  {
    m_window->MoveViewport(-document->CurrentTileSize(), 0);
  }
}

void tactile_app::handle_pan_left()
{
  if (const auto* document = m_model->CurrentDocument())
  {
    m_window->MoveViewport(document->CurrentTileSize(), 0);
  }
}

void tactile_app::handle_new_tileset()
{
  TilesetDialog::Spawn([this](const TilesetInfo& info) {
    m_model->CreateTileset(info.image,
                           info.path,
                           info.name,
                           info.tileWidth,
                           info.tileHeight);
  });
}

void tactile_app::handle_new_map()
{
  const auto id = m_model->AddMap();
  m_window->OnNewMapAdded(m_model->GetDocument(id), id);
}

}  // namespace tactile
