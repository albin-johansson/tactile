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

void window_connections::init(window* window)
{
  m_window = window;
  m_ui = m_window->m_ui.get();

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

  bind(m_ui->actionUndo, &window::ui_undo);
  bind(m_ui->actionRedo, &window::ui_redo);
  bind(m_ui->actionSave, &window::ui_save);
  bind(m_ui->actionNewMap, &window::ui_new_map);
  bind(m_ui->actionAddTileset, &window::ui_add_tileset);
  bind(m_ui->actionAddRow, &window::ui_add_row);
  bind(m_ui->actionAddCol, &window::ui_add_col);
  bind(m_ui->actionRemoveRow, &window::ui_remove_row);
  bind(m_ui->actionRemoveCol, &window::ui_remove_col);
  bind(m_ui->actionResizeMap, &window::ui_resize_map);
  bind(m_ui->actionPanUp, &window::ui_pan_up);
  bind(m_ui->actionPanDown, &window::ui_pan_down);
  bind(m_ui->actionPanRight, &window::ui_pan_right);
  bind(m_ui->actionPanLeft, &window::ui_pan_left);
  bind(m_ui->actionZoomIn, &window::ui_increase_zoom);
  bind(m_ui->actionZoomOut, &window::ui_decrease_zoom);
  bind(m_ui->actionResetZoom, &window::ui_reset_tile_size);
  bind(m_ui->actionCenterCamera, &window::center_viewport);
  bind(m_ui->actionResetLayout, &window::reset_dock_layout);
  bind(m_ui->actionCloseMap, &window::closed_map);
  bind(m_ui->actionTilesetsVisibility, &window::tileset_widget_visibility_changed);
  bind(m_ui->actionToolsVisibility, &window::tool_widget_visibility_changed);
  bind(m_ui->actionLayersVisibility, &window::layer_widget_visibility_changed);
  bind(m_ui->actionPropertiesVisibility, &window::properties_widget_visibility_changed);
  bind(m_ui->actionAbout, &about_dialog::spawn);
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

  bindVisibility(m_window->m_toolDock, m_ui->actionToolsVisibility);
  bindVisibility(m_window->m_tilesetDock, m_ui->actionTilesetsVisibility);
  bindVisibility(m_window->m_layerDock, m_ui->actionLayersVisibility);

  const auto bindClosed = [](auto&& dock, auto&& callable) {
    connect(dock, &tool_dock::closed, callable);
  };

  bindClosed(m_window->m_toolDock, [] { prefs::gfx::tool_widget_visible() = false; });
  bindClosed(m_window->m_layerDock, [] { prefs::gfx::layer_widget_visible() = false; });
  bindClosed(m_window->m_tilesetDock, [] { prefs::gfx::tileset_widget_visible() = false; });
  bindClosed(m_window->m_propertiesDock, [] { prefs::gfx::properties_widget_visible() = false; });
  // clang-format on
}

void window_connections::init_map_editor()
{
  // clang-format off
  auto* editor = m_window->m_editor;
  connect(editor, &map_editor::ui_select_map, m_window, &window::ui_select_map);
  connect(editor, &map_editor::ui_remove_map, m_window, &window::when_about_to_close_map);
  connect(editor, &map_editor::increase_zoom, m_window, &window::ui_increase_zoom);
  connect(editor, &map_editor::decrease_zoom, m_window, &window::ui_decrease_zoom);
  connect(editor, &map_editor::mouse_pressed, m_window, &window::mouse_pressed);
  connect(editor, &map_editor::mouse_released, m_window, &window::mouse_released);
  connect(editor, &map_editor::mouse_entered, m_window, &window::when_mouse_entered);
  connect(editor, &map_editor::mouse_exited, m_window, &window::when_mouse_exited);
  connect(editor, &map_editor::mouse_moved, m_window, &window::when_mouse_moved);
  // clang-format on
}

void window_connections::init_tileset_dock()
{
  // clang-format off
  auto* dock = m_window->m_tilesetDock;
  connect(dock, &tileset_dock::ui_add_tileset, m_window, &window::ui_add_tileset);
  connect(dock, &tileset_dock::ui_select_tileset, m_window, &window::ui_select_tileset);
  connect(dock, &tileset_dock::ui_remove_tileset, m_window, &window::ui_remove_tileset);
  connect(dock, &tileset_dock::ui_rename_tileset, m_window, &window::ui_rename_tileset);
  connect(dock, &tileset_dock::ui_set_tileset_selection, m_window, &window::ui_set_tileset_selection);
  // clang-format on
}

void window_connections::init_tool_dock()
{
  // clang-format off
  auto* dock = m_window->m_toolDock;
  connect(dock, &tool_dock::enable_stamp, m_window, &window::stamp_enabled);
  connect(dock, &tool_dock::enable_bucket, m_window, &window::bucket_enabled);
  connect(dock, &tool_dock::enable_eraser, m_window, &window::eraser_enabled);
  // clang-format on
}

}  // namespace tactile::gui