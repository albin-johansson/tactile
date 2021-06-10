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

namespace tactile::gui {

void window_connections::init(Window* window)
{
  m_window = window;
  m_ui = m_window->mUi.get();

  init_actions();
  init_dock_connections();
  init_map_editor();
  init_tileset_dock();
  init_tool_dock();
}

void window_connections::init_actions()
{
  // clang-format off
  const auto bind = [=](auto&& action, auto&& slot) {
    connect(action, &QAction::triggered, m_window, slot);
  };

  // FIXME connect(m_statusBar, &status_bar::select_layer_request, this, &window::ui_select_layer);

  bind(m_ui->actionUndo,       &Window::S_Undo);
  bind(m_ui->actionRedo,       &Window::S_Redo);
  bind(m_ui->actionSave,       &Window::S_Save);
  bind(m_ui->actionNewMap,     &Window::S_NewMap);
  bind(m_ui->actionAddTileset, &Window::S_AddTileset);
  bind(m_ui->actionAddRow,     &Window::S_AddRow);
  bind(m_ui->actionAddCol,     &Window::S_AddCol);
  bind(m_ui->actionRemoveRow,  &Window::S_RemoveRow);
  bind(m_ui->actionRemoveCol,  &Window::S_RemoveCol);
  bind(m_ui->actionResizeMap,  &Window::S_ResizeMap);
  bind(m_ui->actionPanUp,      &Window::S_PanUp);
  bind(m_ui->actionPanDown,    &Window::S_PanDown);
  bind(m_ui->actionPanRight,   &Window::S_PanRight);
  bind(m_ui->actionPanLeft,    &Window::S_PanLeft);
  bind(m_ui->actionZoomIn,     &Window::S_ZoomIn);
  bind(m_ui->actionZoomOut,    &Window::S_ZoomOut);
  bind(m_ui->actionResetZoom,  &Window::S_ResetZoom);

  bind(m_ui->actionCenterCamera,         &Window::CenterViewport);
  bind(m_ui->actionResetLayout,          &Window::OnResetLayoutAction);
  bind(m_ui->actionOpenMap,              &Window::OnOpenMapAction);
  bind(m_ui->actionCloseMap,             &Window::OnCloseMapAction);
  bind(m_ui->actionTilesetsVisibility,   &Window::OnTilesetWidgetVisibilityChanged);
  bind(m_ui->actionToolsVisibility,      &Window::OnToolWidgetVisibilityChanged);
  bind(m_ui->actionLayersVisibility,     &Window::OnLayerWidgetVisibilityChanged);
  bind(m_ui->actionPropertiesVisibility, &Window::OnPropertiesWidgetVisibilityChanged);
  bind(m_ui->actionSettings,             &Window::OnOpenSettingsAction);
  bind(m_ui->actionToggleGrid,           &Window::OnToggleGridAction);
  bind(m_ui->actionStampTool,            &Window::OnToggleStampAction);
  bind(m_ui->actionEraserTool,           &Window::OnToggleEraserAction);
  bind(m_ui->actionBucketTool,           &Window::OnToggleBucketAction);

  bind(m_ui->actionAbout,   &AboutDialog::Spawn);
  bind(m_ui->actionAboutQt, &QApplication::aboutQt);

  bind(m_ui->actionExit, &QApplication::exit);
  // clang-format on
}

void window_connections::init_dock_connections()
{
  // clang-format off
  const auto bindVisibility = [](auto&& dock, auto&& action) {
    connect(dock, &QDockWidget::visibilityChanged, action, &QAction::setChecked);
  };

  bindVisibility(m_window->mToolDock, m_ui->actionToolsVisibility);
  bindVisibility(m_window->mTilesetDock, m_ui->actionTilesetsVisibility);
  bindVisibility(m_window->mLayerDock, m_ui->actionLayersVisibility);

  const auto bindClosed = [](auto&& dock, auto&& callable) {
    connect(dock, &DockWidget::S_Closed, callable);
  };

  bindClosed(m_window->mToolDock, [] { prefs::gfx::tool_widget_visible() = false; });
  bindClosed(m_window->mLayerDock, [] { prefs::gfx::layer_widget_visible() = false; });
  bindClosed(m_window->mTilesetDock, [] { prefs::gfx::tileset_widget_visible() = false; });
  bindClosed(m_window->mPropertiesDock, [] { prefs::gfx::properties_widget_visible() = false; });
  // clang-format on
}

void window_connections::init_map_editor()
{
  // clang-format off
  auto* editor = m_window->mEditor;
  connect(editor, &MapEditor::S_SelectMap, m_window, &Window::S_SelectMap);
  connect(editor, &MapEditor::S_ZoomIn, m_window, &Window::S_ZoomIn);
  connect(editor, &MapEditor::S_ZoomOut, m_window, &Window::S_ZoomOut);
  connect(editor, &MapEditor::S_MousePressed, m_window, &Window::S_MousePressed);
  connect(editor, &MapEditor::S_MouseReleased, m_window, &Window::S_MouseReleased);

  connect(editor, &MapEditor::S_RemoveMap, m_window, &Window::OnAboutToCloseMap);
  connect(editor, &MapEditor::S_MouseEntered, m_window, &Window::OnMouseEntered);
  connect(editor, &MapEditor::S_MouseExited, m_window, &Window::OnMouseExited);
  connect(editor, &MapEditor::S_MouseMoved, m_window, &Window::OnMouseMoved);
  // clang-format on
}

void window_connections::init_tileset_dock()
{
  // clang-format off
  auto* dock = m_window->mTilesetDock;
  connect(dock, &TilesetDock::S_AddTileset, m_window, &Window::S_AddTileset);
  connect(dock, &TilesetDock::S_SelectTileset, m_window, &Window::S_SelectTileset);
  connect(dock, &TilesetDock::S_RemoveTileset, m_window, &Window::S_RemoveTileset);
  connect(dock, &TilesetDock::S_RenameTileset, m_window, &Window::S_RenameTileset);
  connect(dock, &TilesetDock::S_SetTilesetSelection, m_window, &Window::S_SetTilesetSelection);
  // clang-format on
}

void window_connections::init_tool_dock()
{
  // clang-format off
  auto* dock = m_window->mToolDock;
  connect(dock, &ToolDock::S_EnabledStamp, m_window, &Window::OnStampEnabled);
  connect(dock, &ToolDock::S_EnabledBucket, m_window, &Window::OnBucketEnabled);
  connect(dock, &ToolDock::S_EnabledEraser, m_window, &Window::OnEraserEnabled);
  // clang-format on
}

}  // namespace tactile::gui