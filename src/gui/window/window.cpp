#include "window.hpp"

#include "about_dialog.hpp"
#include "layer_dock.hpp"
#include "map_editor.hpp"
#include "open_map_dialog.hpp"
#include "preferences.hpp"
#include "save_as_dialog.hpp"
#include "save_service.hpp"
#include "setting.hpp"
#include "settings_dialog.hpp"
#include "tileset_dock.hpp"
#include "tool_dock.hpp"
#include "tool_widget.hpp"
#include "ui_window.h"

namespace tactile::gui {

window::window(QWidget* parent) : QMainWindow{parent}, m_ui{new Ui::window{}}
{
  m_ui->setupUi(this);

  m_editor = new map_editor{this};
  m_toolDock = new tool_dock{this};
  m_layerDock = new layer_dock{this};
  m_tilesetDock = new tileset_dock{this};

  setCentralWidget(m_editor);
  addDockWidget(Qt::LeftDockWidgetArea, m_toolDock);

  // FIXME order doesn't seem to matter?
  addDockWidget(Qt::RightDockWidgetArea, m_tilesetDock);
  addDockWidget(Qt::RightDockWidgetArea, m_layerDock);

  init_mouse_tool_group();
  init_connections();

  restore_layout();
  enter_no_content_view();  // TODO option to reopen last map
}

window::~window() noexcept
{
  delete m_ui;
}

void window::init_mouse_tool_group()
{
  Q_ASSERT(!m_toolGroup);

  m_toolGroup = new QActionGroup{this};
  m_toolGroup->setExclusive(true);
  m_toolGroup->addAction(m_ui->actionStampTool);
  m_toolGroup->addAction(m_ui->actionBucketTool);
  m_toolGroup->addAction(m_ui->actionEraserTool);

  // clang-format off
  connect(m_toolDock, &tool_dock::enable_stamp, this, &window::stamp_enabled);
  connect(m_toolDock, &tool_dock::enable_bucket, this, &window::bucket_enabled);
  connect(m_toolDock, &tool_dock::enable_eraser, this, &window::eraser_enabled);
  // clang-format on
}

void window::init_connections()
{
  const auto on_triggered = [this](auto&& sender, auto&& slot) {
    connect(sender, &QAction::triggered, this, slot);
  };

  // clang-format off

  on_triggered(m_ui->actionNewMap, &window::ui_new_map);
  on_triggered(m_ui->actionAddTileset, &window::ui_new_tileset);

  connect(m_toolDock, &QDockWidget::visibilityChanged, m_ui->actionToolsVisibility, &QAction::setChecked);
  connect(m_tilesetDock, &QDockWidget::visibilityChanged, m_ui->actionTilesetsVisibility, &QAction::setChecked);
  connect(m_layerDock, &QDockWidget::visibilityChanged, m_ui->actionLayersVisibility, &QAction::setChecked);

  connect(m_editor, &map_editor::ui_select_map, this, &window::ui_select_map);
  connect(m_editor, &map_editor::ui_remove_map, this, &window::handle_remove_map);
  connect(m_editor, &map_editor::increase_zoom, this, &window::ui_increase_zoom);
  connect(m_editor, &map_editor::decrease_zoom, this, &window::ui_decrease_zoom);
  connect(m_editor, &map_editor::mouse_pressed, this, &window::mouse_pressed);
  connect(m_editor, &map_editor::mouse_moved, this, &window::mouse_moved);
  connect(m_editor, &map_editor::mouse_released, this, &window::mouse_released);
  connect(m_editor, &map_editor::mouse_entered, this, &window::mouse_entered);
  connect(m_editor, &map_editor::mouse_exited, this, &window::mouse_exited);

  connect(m_tilesetDock, &tileset_dock::ui_requested_tileset, this, &window::ui_new_tileset);
  connect(m_tilesetDock, &tileset_dock::ui_selected_tileset, this, &window::ui_selected_tileset);
  connect(m_tilesetDock, &tileset_dock::ui_removed_tileset, this, &window::ui_removed_tileset);
  connect(m_tilesetDock, &tileset_dock::tileset_selection_changed, this, &window::ui_tileset_selection_changed);

  connect(m_layerDock, &layer_dock::ui_requested_new_layer, this, &window::ui_requested_new_layer);
  connect(m_layerDock, &layer_dock::ui_requested_remove_layer, this, &window::ui_requested_remove_layer);
  connect(m_layerDock, &layer_dock::ui_selected_layer, this, &window::ui_selected_layer);
  connect(m_layerDock, &layer_dock::ui_set_layer_visibility, this, &window::ui_set_layer_visibility);

  // clang-format on
}

void window::restore_layout()
{
  if (const auto geometry = prefs::window::last_layout_geometry(); geometry) {
    restoreGeometry(*geometry);
  }

  if (const auto state = prefs::window::last_layout_state(); state) {
    restoreState(*state);
  }
}

void window::reset_dock_layout()
{
  removeDockWidget(m_toolDock);
  removeDockWidget(m_tilesetDock);

  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_toolDock);
  m_toolDock->show();

  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tilesetDock);
  m_tilesetDock->show();

  addDockWidget(Qt::RightDockWidgetArea, m_layerDock);
  m_layerDock->show();
}

void window::hide_all_docks()
{
  m_toolDock->close();
  m_layerDock->close();
  m_tilesetDock->close();
}

void window::show_all_docks()
{
  m_toolDock->show();
  m_layerDock->show();
  m_tilesetDock->show();
}

void window::center_viewport()
{
  m_editor->center_viewport();
}

void window::set_actions_enabled(bool enabled)
{
  // File
  m_ui->actionCloseMap->setEnabled(enabled);
  //  m_ui->action_save->setEnabled(enabled); // TODO uncomment when added
  m_ui->actionSaveAs->setEnabled(enabled);
  //  m_ui->action_rename->setEnabled(enabled); // TODO uncomment when added

  // Edit
  m_ui->actionAddCol->setEnabled(enabled);
  m_ui->actionAddRow->setEnabled(enabled);
  m_ui->actionRemoveRow->setEnabled(enabled);
  m_ui->actionRemoveCol->setEnabled(enabled);
  m_ui->actionResizeMap->setEnabled(enabled);
  m_ui->actionStampTool->setEnabled(enabled);
  m_ui->actionBucketTool->setEnabled(enabled);
  m_ui->actionEraserTool->setEnabled(enabled);
  m_ui->actionAddTileset->setEnabled(enabled);

  // View
  m_ui->actionResetLayout->setEnabled(enabled);
  m_ui->actionToolsVisibility->setEnabled(enabled);
  m_ui->actionTilesetsVisibility->setEnabled(enabled);
  m_ui->actionLayersVisibility->setEnabled(enabled);
  m_ui->actionCenterCamera->setEnabled(enabled);
  m_ui->actionToggleGrid->setEnabled(enabled);
  m_ui->actionZoomIn->setEnabled(enabled);
  m_ui->actionZoomOut->setEnabled(enabled);
  m_ui->actionResetZoom->setEnabled(enabled);
  m_ui->actionPanUp->setEnabled(enabled);
  m_ui->actionPanDown->setEnabled(enabled);
  m_ui->actionPanRight->setEnabled(enabled);
  m_ui->actionPanLeft->setEnabled(enabled);
}

auto window::in_editor_mode() const -> bool
{
  return m_editor->in_editor_mode();
}

void window::enter_no_content_view()
{
  m_editor->enable_startup_view();
  m_toolDock->disable_tools();

  set_actions_enabled(false);
  hide_all_docks();
}

void window::enter_content_view()
{
  m_editor->enable_editor_view();
  m_toolDock->enable_tools();

  set_actions_enabled(true);
}

void window::force_redraw()
{
  m_editor->force_redraw();
}

void window::undo_state_updated(bool canUndo)
{
  m_ui->actionUndo->setEnabled(canUndo);
}

void window::redo_state_updated(bool canRedo)
{
  m_ui->actionRedo->setEnabled(canRedo);
}

void window::undo_text_updated(const QString& text)
{
  m_ui->actionUndo->setText(QStringLiteral(u"Undo ") + text);
}

void window::redo_text_updated(const QString& text)
{
  m_ui->actionRedo->setText(QStringLiteral(u"Redo ") + text);
}

void window::added_tileset(map_id map,
                           tileset_id id,
                           const core::tileset& tileset)
{
  m_tilesetDock->added_tileset(map, id, tileset);
}

void window::removed_tileset(tileset_id id)
{
  m_tilesetDock->removed_tileset(id);
}

void window::selected_layer(layer_id id, const core::layer& layer)
{
  m_layerDock->selected_layer(id, layer);
}

void window::added_layer(layer_id id, const core::layer& layer)
{
  m_layerDock->added_layer(id, layer);
}

void window::removed_layer(layer_id id)
{
  m_layerDock->removed_layer(id);
}

void window::switched_map(map_id id, const core::map_document& document)
{
  m_tilesetDock->selected_map(id);
  m_layerDock->selected_map(document);
}

void window::enable_stamp_preview(const core::position& position)
{
  m_editor->enable_stamp_preview(position);
}

void window::disable_stamp_preview()
{
  m_editor->disable_stamp_preview();
}

void window::handle_move_camera(int dx, int dy)
{
  m_editor->move_map(dx, dy);
}

void window::handle_new_map(core::map_document* map, map_id id)
{
  m_editor->add_map_tab(map, id, QStringLiteral(u"map"));
  m_editor->select_tab(id);
  if (!in_editor_mode()) {
    enter_content_view();
    show_all_docks();  // TODO just reopen docks that were visible
    center_viewport();
  }
}

void window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);
  prefs::window::last_layout_geometry().set(saveGeometry());
  prefs::window::last_layout_state().set(saveState());
}

void window::handle_remove_map(map_id tabID)
{
  emit ui_close_map(tabID);

  // The tab isn't actually removed yet, this checks if there will be
  // no open tabs
  if (m_editor->tab_count() == 1) {
    enter_no_content_view();
  }
}

void window::handle_theme_changed()
{
  emit m_editor->theme_changed();
}

void window::stamp_enabled()
{
  m_ui->actionStampTool->setChecked(true);
  emit ui_selected_tool(tool_id::stamp);
}

void window::bucket_enabled()
{
  m_ui->actionBucketTool->setChecked(true);
  emit ui_selected_tool(tool_id::bucket);
}

void window::eraser_enabled()
{
  m_ui->actionEraserTool->setChecked(true);
  emit ui_selected_tool(tool_id::eraser);
}

void window::on_actionUndo_triggered()
{
  emit ui_undo();
}

void window::on_actionRedo_triggered()
{
  emit ui_redo();
}

void window::on_actionCloseMap_triggered()
{
  // TODO save current state of open map
  const auto id = m_editor->active_tab_id().value();

  m_editor->close_tab(id);
  emit ui_close_map(id);

  if (m_editor->tab_count() == 0) {
    enter_no_content_view();
  }
}

void window::on_actionTilesetsVisibility_triggered()
{
  m_tilesetDock->setVisible(m_ui->actionTilesetsVisibility->isChecked());
}

void window::on_actionToolsVisibility_triggered()
{
  m_toolDock->setVisible(m_ui->actionToolsVisibility->isChecked());
}

void window::on_actionLayersVisibility_triggered()
{
  m_layerDock->setVisible(m_ui->actionLayersVisibility->isChecked());
}

void window::on_actionSave_triggered()
{
  // TODO
}

void window::on_actionSaveAs_triggered()
{
  auto get_tab_name = [this]() -> QString {
    if (const auto name = m_editor->active_tab_name(); name) {
      return *name;
    } else {
      return QStringLiteral(u"map");
    }
  };
  save_as_dialog::spawn(
      [this](const QString& path) {
        emit ui_save_as(path);
      },
      get_tab_name());
}

void window::on_actionOpenMap_triggered()
{
  open_map_dialog::spawn([this](const QString& path) {
    emit ui_open_map(path);
  });
}

void window::on_actionRename_triggered()
{
  // TODO
}

void window::on_actionAddRow_triggered()
{
  emit ui_add_row();
}

void window::on_actionAddCol_triggered()
{
  emit ui_add_col();
}

void window::on_actionRemoveRow_triggered()
{
  emit ui_remove_row();
}

void window::on_actionRemoveCol_triggered()
{
  emit ui_remove_col();
}

void window::on_actionResizeMap_triggered()
{
  emit ui_resize_map();
}

void window::on_actionToggleGrid_triggered()
{
  if (auto grid = prefs::graphics::render_grid(); grid) {
    grid.set(!*grid);
    force_redraw();
  }
}

void window::on_actionPanUp_triggered()
{
  emit ui_pan_up();
}

void window::on_actionPanDown_triggered()
{
  emit ui_pan_down();
}

void window::on_actionPanRight_triggered()
{
  emit ui_pan_right();
}

void window::on_actionPanLeft_triggered()
{
  emit ui_pan_left();
}

void window::on_actionZoomIn_triggered()
{
  emit ui_increase_zoom();
}

void window::on_actionZoomOut_triggered()
{
  emit ui_decrease_zoom();
}

void window::on_actionResetZoom_triggered()
{
  emit ui_reset_tile_size();
}

void window::on_actionCenterCamera_triggered()
{
  center_viewport();
}

void window::on_actionResetLayout_triggered()
{
  reset_dock_layout();
}

void window::on_actionStampTool_triggered()
{
  m_toolDock->stamp_enabled();
}

void window::on_actionBucketTool_triggered()
{
  m_toolDock->bucket_enabled();
}

void window::on_actionEraserTool_triggered()
{
  m_toolDock->eraser_enabled();
}

void window::on_actionSettings_triggered()  // NOLINT
{
  settings_dialog settings;

  // clang-format off
  connect(&settings, &settings_dialog::reload_theme, this, &window::handle_theme_changed);
  // clang-format on

  settings.exec();
}

void window::on_actionAboutQt_triggered()
{
  QApplication::aboutQt();
}

void window::on_actionExit_triggered()
{
  QApplication::exit();
}

void window::on_actionAbout_triggered()
{
  about_dialog about;
  about.exec();
}

}  // namespace tactile::gui
