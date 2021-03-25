#include "window.hpp"

#include "init_ui.hpp"
#include "layer_dock.hpp"
#include "map_document.hpp"
#include "map_editor.hpp"
#include "open_map_dialog.hpp"
#include "preferences.hpp"
#include "properties_dock.hpp"
#include "property_model.hpp"
#include "save_as_dialog.hpp"
#include "setting.hpp"
#include "settings_dialog.hpp"
#include "status_bar.hpp"
#include "tactile_qstring.hpp"
#include "tileset_dock.hpp"
#include "tool_dock.hpp"
#include "ui_window.h"
#include "window_connections.hpp"

namespace tactile::gui {

window::window(QWidget* parent)
    : QMainWindow{parent}
    , m_ui{init_ui<Ui::window>(this)}
    , m_editor{new map_editor{this}}
    , m_toolDock{new tool_dock{this}}
    , m_layerDock{new layer_dock{this}}
    , m_tilesetDock{new tileset_dock{this}}
    , m_propertiesDock{new properties_dock{this}}
    , m_statusBar{new status_bar{this}}
    , m_toolGroup{new QActionGroup{this}}
{
  setContentsMargins(0, 0, 0, 0);

  setCentralWidget(m_editor);
  addDockWidget(Qt::LeftDockWidgetArea, m_propertiesDock);
  addDockWidget(Qt::RightDockWidgetArea, m_tilesetDock);
  addDockWidget(Qt::RightDockWidgetArea, m_layerDock);
  setStatusBar(m_statusBar);

  m_toolGroup->setExclusive(true);
  m_toolGroup->addAction(m_ui->actionStampTool);
  m_toolGroup->addAction(m_ui->actionBucketTool);
  m_toolGroup->addAction(m_ui->actionEraserTool);

  window_connections connections;
  connections.init(this);

  showMaximized();

  restore_layout();
  enter_no_content_view();
}

window::~window() noexcept = default;

void window::restore_layout()
{
  prefs::window::last_layout_geometry().with([this](const QByteArray& geom) {
    restoreGeometry(geom);
  });

  prefs::window::last_layout_state().with([this](const QByteArray& state) {
    restoreState(state);
  });
}

void window::save_as()
{
  save_as_dialog::spawn(
      [this](const QString& path) {
        emit ui_save_as(path);
      },
      m_editor->active_tab_name().value_or(TACTILE_QSTRING(u"map")));
}

void window::reset_dock_layout()
{
  removeDockWidget(m_toolDock);
  removeDockWidget(m_tilesetDock);
  removeDockWidget(m_layerDock);
  removeDockWidget(m_propertiesDock);

  addDockWidget(Qt::LeftDockWidgetArea, m_toolDock);
  addDockWidget(Qt::RightDockWidgetArea, m_tilesetDock);
  addDockWidget(Qt::RightDockWidgetArea, m_layerDock);
  addDockWidget(Qt::LeftDockWidgetArea, m_propertiesDock);

  prefs::gfx::reset_tool_widget_visible();
  prefs::gfx::reset_tileset_widget_visible();
  prefs::gfx::reset_layer_widget_visible();
  prefs::gfx::reset_properties_widget_visible();

  m_toolDock->setVisible(prefs::gfx::tool_widget_visible().value());
  m_tilesetDock->setVisible(prefs::gfx::tileset_widget_visible().value());
  m_layerDock->setVisible(prefs::gfx::layer_widget_visible().value());
  m_propertiesDock->setVisible(prefs::gfx::properties_widget_visible().value());
}

void window::hide_all_docks()
{
  m_toolDock->hide();
  m_layerDock->hide();
  m_tilesetDock->hide();
  m_propertiesDock->hide();
}

void window::restore_dock_visibility()
{
  prefs::gfx::tool_widget_visible().with([this](const bool value) {
    m_toolDock->setVisible(value);
  });

  prefs::gfx::tileset_widget_visible().with([this](const bool value) {
    m_tilesetDock->setVisible(value);
  });

  prefs::gfx::layer_widget_visible().with([this](const bool value) {
    m_layerDock->setVisible(value);
  });

  prefs::gfx::properties_widget_visible().with([this](const bool value) {
    m_propertiesDock->setVisible(value);
  });
}

void window::center_viewport()
{
  m_editor->center_viewport();
}

void window::set_actions_enabled(const bool enabled)
{
  // File
  m_ui->actionCloseMap->setEnabled(enabled);
  m_ui->actionSave->setEnabled(enabled);
  m_ui->actionSaveAs->setEnabled(enabled);

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

  m_statusBar->set_layer_combo_box_visible(false);
}

void window::enter_content_view()
{
  m_editor->enable_editor_view();
  m_toolDock->enable_tools();

  set_actions_enabled(true);
  restore_dock_visibility();

  m_statusBar->set_layer_combo_box_visible(true);
}

void window::trigger_save_as()
{
  save_as();
}

void window::set_active_tab_name(const QString& name)
{
  m_editor->set_active_tab_name(name);
}

void window::force_redraw()
{
  m_editor->force_redraw();
}

void window::undo_state_updated(const bool canUndo)
{
  m_ui->actionUndo->setEnabled(canUndo);
}

void window::redo_state_updated(const bool canRedo)
{
  m_ui->actionRedo->setEnabled(canRedo);
}

void window::undo_text_updated(const QString& text)
{
  m_ui->actionUndo->setText(TACTILE_QSTRING(u"Undo ") + text);
}

void window::redo_text_updated(const QString& text)
{
  m_ui->actionRedo->setText(TACTILE_QSTRING(u"Redo ") + text);
}

void window::clean_changed(const bool clean)
{
  m_ui->actionSave->setDisabled(clean);
}

void window::added_tileset(const map_id map,
                           const tileset_id id,
                           const core::tileset& tileset)
{
  m_tilesetDock->added_tileset(map, id, tileset);
}

void window::removed_tileset(const tileset_id id)
{
  m_tilesetDock->removed_tileset(id);
}

void window::renamed_tileset(const tileset_id id, const QString& name)
{
  m_tilesetDock->renamed_tileset(id, name);
}

void window::selected_layer(const layer_id id, const core::layer& layer)
{
  m_statusBar->set_current_layer(id);
}

void window::added_layer(const layer_id id, const core::layer& layer)
{
  m_statusBar->added_layer(id, layer.name());
}

void window::added_duplicated_layer(const layer_id id, const core::layer& layer)
{
  m_statusBar->added_layer(id, layer.name());
}

void window::removed_layer(const layer_id id)
{
  m_statusBar->removed_layer(id);
}

void window::added_property(const QString& name)
{
  m_propertiesDock->added_property(name);
}

void window::about_to_remove_property(const QString& name)
{
  m_propertiesDock->about_to_remove_property(name);
}

void window::updated_property(const QString& name)
{
  m_propertiesDock->updated_property(name);
}

void window::changed_property_type(const QString& name)
{
  m_propertiesDock->changed_property_type(name);
}

void window::renamed_property(const QString& oldName, const QString& newName)
{
  m_propertiesDock->renamed_property(oldName, newName);
}

void window::enable_stamp_preview(const core::position& position)
{
  m_editor->enable_stamp_preview(position);
}

void window::disable_stamp_preview()
{
  m_editor->disable_stamp_preview();
}

void window::handle_move_camera(const int dx, const int dy)
{
  m_editor->move_map(dx, dy);
}

void window::switched_map(const map_id id,
                          not_null<core::map_document*> document)
{
  m_tilesetDock->switched_map(id);
  m_layerDock->switched_map(document);
  m_propertiesDock->switched_map(document);
  m_statusBar->switched_map(*document);
}

void window::when_new_map_added(not_null<core::map_document*> document,
                                const map_id id,
                                const QString& name)
{
  Q_ASSERT(document);

  m_editor->add_map_tab(document, id, name);
  m_editor->select_tab(id);
  if (!in_editor_mode())
  {
    enter_content_view();
    center_viewport();
  }

  m_ui->actionSave->setDisabled(document->is_clean());
  m_tilesetDock->added_map(id, *document);
  m_layerDock->switched_map(document);
  m_propertiesDock->switched_map(document);
  m_statusBar->switched_map(*document);
}

void window::show_map_properties(not_null<core::property_manager*> manager)
{
  m_propertiesDock->show_map_properties(manager);
}

void window::show_layer_properties(not_null<core::property_manager*> manager)
{
  m_propertiesDock->show_layer_properties(manager);
}

void window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);
  prefs::window::last_layout_geometry() = saveGeometry();
  prefs::window::last_layout_state() = saveState();
}

void window::when_about_to_close_map(const map_id id)
{
  emit ui_about_to_close_map(id);
  if (m_editor->tab_count() == 1)
  {
    enter_no_content_view();
  }
}

void window::handle_theme_changed()
{
  emit m_editor->theme_changed();
}

void window::handle_reload_opengl(const bool enabled)
{
  m_editor->set_opengl_enabled(enabled);
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

void window::closed_map()
{
  // TODO save current state of open map (exit saves?)
  const auto id = m_editor->active_tab_id().value();

  m_editor->close_tab(id);
  emit ui_about_to_close_map(id);

  if (m_editor->tab_count() == 0)
  {
    enter_no_content_view();
  }
}

void window::tileset_widget_visibility_changed()
{
  const auto visible = m_ui->actionTilesetsVisibility->isChecked();
  m_tilesetDock->setVisible(visible);
  prefs::gfx::tileset_widget_visible() = visible;
}

void window::tool_widget_visibility_changed()
{
  const auto visible = m_ui->actionToolsVisibility->isChecked();
  m_toolDock->setVisible(visible);
  prefs::gfx::tool_widget_visible() = visible;
}

void window::layer_widget_visibility_changed()
{
  const auto visible = m_ui->actionLayersVisibility->isChecked();
  m_layerDock->setVisible(visible);
  prefs::gfx::layer_widget_visible() = visible;
}

void window::properties_widget_visibility_changed()
{
  const auto visible = m_ui->actionPropertiesVisibility->isChecked();
  m_propertiesDock->setVisible(visible);
  prefs::gfx::properties_widget_visible() = visible;
}

void window::when_mouse_entered(QEvent* event)
{
  m_statusBar->set_mouse_info_visible(true);
  emit mouse_entered(event);
}

void window::when_mouse_exited(QEvent* event)
{
  m_statusBar->set_mouse_info_visible(false);
  emit mouse_exited(event);
}

void window::when_mouse_moved(QMouseEvent* event, const QPointF mapPos)
{
  m_statusBar->mouse_moved(event->pos() - mapPos);
  emit mouse_moved(event, mapPos);
}

[[maybe_unused]] void window::on_actionOpenMap_triggered()
{
  open_map_dialog::spawn([this](const QString& path) {
    emit ui_open_map(path);
  });
}

[[maybe_unused]] void window::on_actionToggleGrid_triggered()
{
  auto grid = prefs::gfx::render_grid();
  grid.with([&, this](bool value) {
    grid = !value;
    force_redraw();
  });
}

[[maybe_unused]] void window::on_actionStampTool_triggered()
{
  m_toolDock->stamp_enabled();
}

[[maybe_unused]] void window::on_actionBucketTool_triggered()
{
  m_toolDock->bucket_enabled();
}

[[maybe_unused]] void window::on_actionEraserTool_triggered()
{
  m_toolDock->eraser_enabled();
}

[[maybe_unused]] void window::on_actionSettings_triggered()
{
  settings_dialog settings{this};

  // clang-format off
  connect(&settings, &settings_dialog::reload_theme,
          this, &window::handle_theme_changed);

  connect(&settings, &settings_dialog::reload_opengl,
          this, &window::handle_reload_opengl);
  // clang-format on

  settings.exec();
}

}  // namespace tactile::gui
