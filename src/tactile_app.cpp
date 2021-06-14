#include "tactile_app.hpp"

#include "model.hpp"
#include "open_map_document.hpp"
#include "open_map_error_dialog.hpp"
#include "parse_error.hpp"
#include "prepare_application.hpp"
#include "resize_dialog.hpp"
#include "save_map_document.hpp"
#include "tileset_dialog.hpp"
#include "window.hpp"

namespace tactile {

TactileApp::TactileApp(int argc, char** argv)
    : QApplication{argc, argv}
    , mModel{std::make_unique<core::Model>()}
{
  PrepareApplication();

  // It is necessary to create the window after calling PrepareApplication
  mWindow = std::make_unique<Window>();
  InitConnections();

  mWindow->show();
}

TactileApp::~TactileApp() noexcept = default;

void TactileApp::InitConnections()
{
  using Model = core::Model;

  const auto modToWin = [this](auto&& sender, auto&& receiver) {
    connect(mModel.get(), sender, mWindow.get(), receiver);
  };

  const auto winToMod = [this](auto&& sender, auto&& receiver) {
    connect(mWindow.get(), sender, mModel.get(), receiver);
  };

  const auto fromWindow = [this](auto&& sender, auto&& receiver) {
    connect(mWindow.get(), sender, this, receiver);
  };

  // clang-format off

  winToMod(&Window::S_Undo,                  &Model::Undo);
  winToMod(&Window::S_Redo,                  &Model::Redo);
  winToMod(&Window::S_AddRow,                &Model::AddRow);
  winToMod(&Window::S_AddCol,                &Model::AddColumn);
  winToMod(&Window::S_RemoveRow,             &Model::RemoveRow);
  winToMod(&Window::S_RemoveCol,             &Model::RemoveColumn);
  winToMod(&Window::S_AboutToCloseMap,       &Model::CloseMap);
  winToMod(&Window::S_SelectMap,             &Model::SelectMap);
  winToMod(&Window::S_ZoomIn,                &Model::IncreaseTileSize);
  winToMod(&Window::S_ZoomOut,               &Model::DecreaseTileSize);
  winToMod(&Window::S_ResetZoom,             &Model::ResetTileSize);
  winToMod(&Window::S_SelectedTool,          &Model::SelectTool);
  winToMod(&Window::S_RemoveTileset,         &Model::RemoveTileset);
  winToMod(&Window::S_SelectTileset,         &Model::SelectTileset);
  winToMod(&Window::S_RenameTileset,         &Model::SetTilesetName);
  winToMod(&Window::S_SetTilesetSelection,   &Model::SetTilesetSelection);

  winToMod(&Window::S_ShowMapProperties,     &Model::OnShowMapProperties);
  winToMod(&Window::S_ShowLayerProperties,   &Model::OnShowLayerProperties);
  winToMod(&Window::S_ShowTilesetProperties, &Model::OnShowTilesetProperties);

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
  modToWin(&Model::S_ShowTilesetProperties, &Window::OnShowTilesetProperties);

  fromWindow(&Window::S_AddTileset, &TactileApp::OnAddTileset);
  fromWindow(&Window::S_ResizeMap,  &TactileApp::OnResizeMap);
  fromWindow(&Window::S_PanUp,      &TactileApp::OnPanUp);
  fromWindow(&Window::S_PanDown,    &TactileApp::OnPanDown);
  fromWindow(&Window::S_PanRight,   &TactileApp::OnPanRight);
  fromWindow(&Window::S_PanLeft,    &TactileApp::OnPanLeft);
  fromWindow(&Window::S_NewMap,     &TactileApp::OnNewMap);
  fromWindow(&Window::S_Save,       &TactileApp::OnSave);
  fromWindow(&Window::S_SaveAs,     &TactileApp::OnSaveAs);
  fromWindow(&Window::S_OpenMap,    &TactileApp::OnOpenMap);
  // clang-format on
}

void TactileApp::OnSave()
{
  if (auto* document = mModel->CurrentDocument())
  {
    if (document->HasPath())
    {
      SaveMapDocument(document->AbsolutePath(), *document);
      document->MarkAsClean();
    }
    else
    {
      mWindow->TriggerSaveAs();
    }
  }
}

void TactileApp::OnSaveAs(const QString& path)
{
  if (auto* document = mModel->CurrentDocument())
  {
    SaveMapDocument(path, *document);

    const QFileInfo file{path};
    document->MarkAsClean();
    document->SetPath(file);

    mWindow->SetActiveTabName(file.baseName());
  }
}

void TactileApp::OnOpenMap(const QString& path)
{
  ParseError error;
  if (auto* document = OpenMapDocument(path, error))
  {
    const auto id = mModel->AddMap(document);
    mWindow->OnNewMapAdded(document, id, QFileInfo{path}.baseName());
  }
  else
  {
    OpenMapErrorDialog dialog{mWindow.get()};
    dialog.SetFile(QFileInfo{path});
    dialog.SetErrorMessage(ToMessage(error));
    dialog.exec();
  }
}

void TactileApp::OnResizeMap()
{
  if (mModel->HasActiveMap())
  {
    ResizeDialog::Spawn([this](const row_t rows, const col_t cols) {
      mModel->ResizeMap(rows, cols);
    });
  }
}

void TactileApp::OnPanUp()
{
  if (const auto* document = mModel->CurrentDocument())
  {
    mWindow->MoveViewport(0, document->CurrentTileSize());
  }
}

void TactileApp::OnPanDown()
{
  if (const auto* document = mModel->CurrentDocument())
  {
    mWindow->MoveViewport(0, -document->CurrentTileSize());
  }
}

void TactileApp::OnPanRight()
{
  if (const auto* document = mModel->CurrentDocument())
  {
    mWindow->MoveViewport(-document->CurrentTileSize(), 0);
  }
}

void TactileApp::OnPanLeft()
{
  if (const auto* document = mModel->CurrentDocument())
  {
    mWindow->MoveViewport(document->CurrentTileSize(), 0);
  }
}

void TactileApp::OnAddTileset()
{
  TilesetDialog::Spawn([this](const TilesetInfo& info) {
    mModel->CreateTileset(info.image,
                          info.path,
                          info.name,
                          info.tileWidth,
                          info.tileHeight);
  });
}

void TactileApp::OnNewMap()
{
  const auto id = mModel->AddMap();
  mWindow->OnNewMapAdded(mModel->GetDocument(id), id);
}

}  // namespace tactile
