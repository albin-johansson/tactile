#include "window_connections.hpp"

#include <QDockWidget>  // QDockWidget

#include "about_dialog.hpp"
#include "layer_dock.hpp"
#include "map_editor.hpp"
#include "preferences.hpp"
#include "properties_dock.hpp"
#include "status_bar.hpp"
#include "tileset_dock.hpp"
#include "tool_dock.hpp"
#include "ui_window.h"
#include "window.hpp"

namespace tactile {

void WindowConnections::Init(Window* window)
{
  mWindow = window;
  mUi = mWindow->mUi.get();

  InitActions();
  InitDockConnections();
  InitMapEditor();
  InitTilesetDock();
  InitToolDock();
}

void WindowConnections::InitActions()
{
  // clang-format off
  const auto bind = [=](auto&& action, auto&& slot) {
    connect(action, &QAction::triggered, mWindow, slot);
  };

  // FIXME connect(m_statusBar, &status_bar::select_layer_request, this, &window::ui_select_layer);

  bind(mUi->actionUndo,       &Window::S_Undo);
  bind(mUi->actionRedo,       &Window::S_Redo);
  bind(mUi->actionSave,       &Window::S_Save);
  bind(mUi->actionSaveAs,     &Window::SaveAs);
  bind(mUi->actionNewMap,     &Window::S_NewMap);
  bind(mUi->actionAddTileset, &Window::S_AddTileset);
  bind(mUi->actionAddRow,     &Window::S_AddRow);
  bind(mUi->actionAddCol,     &Window::S_AddCol);
  bind(mUi->actionRemoveRow,  &Window::S_RemoveRow);
  bind(mUi->actionRemoveCol,  &Window::S_RemoveCol);
  bind(mUi->actionResizeMap,  &Window::S_ResizeMap);
  bind(mUi->actionPanUp,      &Window::S_PanUp);
  bind(mUi->actionPanDown,    &Window::S_PanDown);
  bind(mUi->actionPanRight,   &Window::S_PanRight);
  bind(mUi->actionPanLeft,    &Window::S_PanLeft);
  bind(mUi->actionZoomIn,     &Window::S_ZoomIn);
  bind(mUi->actionZoomOut,    &Window::S_ZoomOut);
  bind(mUi->actionResetZoom,  &Window::S_ResetZoom);

  bind(mUi->actionCenterCamera,         &Window::CenterViewport);
  bind(mUi->actionResetLayout,          &Window::OnResetLayoutAction);
  bind(mUi->actionOpenMap,              &Window::OnOpenMapAction);
  bind(mUi->actionCloseMap,             &Window::OnCloseMapAction);
  bind(mUi->actionTilesetsVisibility,   &Window::OnTilesetWidgetVisibilityChanged);
  bind(mUi->actionToolsVisibility,      &Window::OnToolWidgetVisibilityChanged);
  bind(mUi->actionLayersVisibility,     &Window::OnLayerWidgetVisibilityChanged);
  bind(mUi->actionPropertiesVisibility, &Window::OnPropertiesWidgetVisibilityChanged);
  bind(mUi->actionSettings,             &Window::OnOpenSettingsAction);
  bind(mUi->actionToggleGrid,           &Window::OnToggleGridAction);
  bind(mUi->actionStampTool,            &Window::OnToggleStampAction);
  bind(mUi->actionEraserTool,           &Window::OnToggleEraserAction);
  bind(mUi->actionBucketTool,           &Window::OnToggleBucketAction);

  bind(mUi->actionAbout,   &AboutDialog::Spawn);
  bind(mUi->actionAboutQt, &QApplication::aboutQt);

  bind(mUi->actionExit, &QApplication::exit);
  // clang-format on
}

void WindowConnections::InitDockConnections()
{
  // clang-format off
  const auto bindVisibility = [](auto&& dock, auto&& action) {
    connect(dock, &QDockWidget::visibilityChanged, action, &QAction::setChecked);
  };

  bindVisibility(mWindow->mToolDock, mUi->actionToolsVisibility);
  bindVisibility(mWindow->mTilesetDock, mUi->actionTilesetsVisibility);
  bindVisibility(mWindow->mLayerDock, mUi->actionLayersVisibility);

  const auto bindClosed = [](auto&& dock, auto&& callable) {
    connect(dock, &DockWidget::S_Closed, callable);
  };

  bindClosed(mWindow->mToolDock, [] { prefs::ToolWidgetVisibility() = false; });
  bindClosed(mWindow->mLayerDock, [] { prefs::LayerWidgetVisibility() = false; });
  bindClosed(mWindow->mTilesetDock, [] { prefs::TilesetWidgetVisibility() = false; });
  bindClosed(mWindow->mPropertiesDock, [] { prefs::PropertiesWidgetVisibility() = false; });
  // clang-format on
}

void WindowConnections::InitMapEditor()
{
  // clang-format off
  auto* editor = mWindow->mEditor;
  connect(editor, &MapEditor::S_SelectMap, mWindow, &Window::S_SelectMap);
  connect(editor, &MapEditor::S_ZoomIn, mWindow, &Window::S_ZoomIn);
  connect(editor, &MapEditor::S_ZoomOut, mWindow, &Window::S_ZoomOut);
  connect(editor, &MapEditor::S_MousePressed, mWindow, &Window::S_MousePressed);
  connect(editor, &MapEditor::S_MouseReleased, mWindow, &Window::S_MouseReleased);

  connect(editor, &MapEditor::S_RemoveMap, mWindow, &Window::OnAboutToCloseMap);
  connect(editor, &MapEditor::S_MouseEntered, mWindow, &Window::OnMouseEntered);
  connect(editor, &MapEditor::S_MouseExited, mWindow, &Window::OnMouseExited);
  connect(editor, &MapEditor::S_MouseMoved, mWindow, &Window::OnMouseMoved);
  // clang-format on
}

void WindowConnections::InitTilesetDock()
{
  // clang-format off
  auto* dock = mWindow->mTilesetDock;
  connect(dock, &TilesetDock::S_AddTileset, mWindow, &Window::S_AddTileset);
  connect(dock, &TilesetDock::S_SelectTileset, mWindow, &Window::S_SelectTileset);
  connect(dock, &TilesetDock::S_RemoveTileset, mWindow, &Window::S_RemoveTileset);
  connect(dock, &TilesetDock::S_RenameTileset, mWindow, &Window::S_RenameTileset);
  connect(dock, &TilesetDock::S_ShowProperties, mWindow, &Window::S_ShowTilesetProperties);
  connect(dock, &TilesetDock::S_SetTilesetSelection, mWindow, &Window::S_SetTilesetSelection);
  // clang-format on
}

void WindowConnections::InitToolDock()
{
  // clang-format off
  auto* dock = mWindow->mToolDock;
  connect(dock, &ToolDock::S_EnabledStamp, mWindow, &Window::OnStampEnabled);
  connect(dock, &ToolDock::S_EnabledBucket, mWindow, &Window::OnBucketEnabled);
  connect(dock, &ToolDock::S_EnabledEraser, mWindow, &Window::OnEraserEnabled);
  // clang-format on
}

}  // namespace tactile