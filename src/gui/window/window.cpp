#include "window.hpp"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QSpacerItem>

#include "about_dialog.hpp"
#include "central_editor_widget.hpp"
#include "mouse_tool_widget.hpp"
#include "setting.hpp"
#include "setting_identifiers.hpp"
#include "settings_dialog.hpp"
#include "ui_window.h"

namespace tactile::gui {

window::window(QWidget* parent) : QMainWindow{parent}, m_ui{new Ui::window{}}
{
  m_ui->setupUi(this);

  // TODO add mini-map widget

  m_centralWidget = new central_editor_widget{this};
  m_toolDock = new tool_dock{this};
  m_tilesetDock = new tileset_dock{this};

  setCentralWidget(m_centralWidget);
  addDockWidget(Qt::LeftDockWidgetArea, m_toolDock);
  addDockWidget(Qt::RightDockWidgetArea, m_tilesetDock);

  init_connections();
  init_layout();
  enable_startup_view();  // TODO option to reopen last tilemap
}

window::~window() noexcept
{
  delete m_ui;
}

void window::init_connections() noexcept
{
  on_triggered(m_ui->action_new_map, &window::request_new_map);
  on_triggered(m_ui->action_add_tileset, &window::request_new_tileset);

  connect(m_centralWidget,
          &central_editor_widget::request_remove_tab,
          this,
          &window::handle_remove_tab);

  {
    m_toolGroup = new QActionGroup{this};
    m_toolGroup->setExclusive(true);
    m_toolGroup->addAction(m_ui->action_stamp_tool);
    m_toolGroup->addAction(m_ui->action_bucket_tool);
    m_toolGroup->addAction(m_ui->action_find_same_tool);
    m_toolGroup->addAction(m_ui->action_eraser_tool);
    m_toolGroup->addAction(m_ui->action_rectangle_tool);

    connect(m_toolDock, &tool_dock::stamp_enabled, this, [this] {
      m_ui->action_stamp_tool->setChecked(true);
    });

    connect(m_toolDock, &tool_dock::bucket_enabled, this, [this] {
      m_ui->action_bucket_tool->setChecked(true);
    });

    connect(m_toolDock, &tool_dock::eraser_enabled, this, [this] {
      m_ui->action_eraser_tool->setChecked(true);
    });

    connect(m_toolDock, &tool_dock::rectangle_enabled, this, [this] {
      m_ui->action_rectangle_tool->setChecked(true);
    });

    connect(m_toolDock, &tool_dock::find_same_enabled, this, [this] {
      m_ui->action_find_same_tool->setChecked(true);
    });
  }

  connect(m_toolDock,
          &QDockWidget::visibilityChanged,
          m_ui->action_mouse_tools_visibility,
          &QAction::setChecked);

  connect(m_tilesetDock,
          &QDockWidget::visibilityChanged,
          m_ui->action_tilesets_visibility,
          &QAction::setChecked);

  connect(m_centralWidget,
          &central_editor_widget::request_redraw,
          this,
          &window::request_redraw);

  connect(m_centralWidget,
          &central_editor_widget::request_select_tab,
          this,
          &window::request_select_map);

  connect(m_tilesetDock,
          &tileset_dock::new_tileset_requested,
          this,
          &window::request_new_tileset);
}

void window::enable_startup_view() noexcept
{
  m_centralWidget->enable_startup_view();
  m_toolDock->get_tool_widget()->disable_tools();
  hide_all_docks();
}

void window::enable_editor_view() noexcept
{
  m_centralWidget->enable_editor_view();

  // FIXME only enable relevant tools
  m_toolDock->get_tool_widget()->enable_tools();
}

void window::init_layout() noexcept
{
  if (setting<QByteArray> geometry{cfg::window::last_layout_geometry()};
      geometry) {
    restoreGeometry(*geometry);
  }

  if (setting<QByteArray> state{cfg::window::last_layout_state()}; state) {
    restoreState(*state);
  }
}

void window::reset_dock_layout() noexcept
{
  removeDockWidget(m_toolDock);
  removeDockWidget(m_tilesetDock);

  addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_toolDock);
  m_toolDock->show();

  addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_tilesetDock);
  m_tilesetDock->show();
}

void window::hide_all_docks() noexcept
{
  m_toolDock->close();
  m_tilesetDock->close();
}

void window::show_all_docks() noexcept
{
  m_toolDock->show();
  m_tilesetDock->show();
}

auto window::in_editor_mode() const noexcept -> bool
{
  return m_centralWidget->in_editor_mode();
}

void window::handle_undo_state_update(bool canUndo)
{
  m_ui->action_undo->setEnabled(canUndo);
}

void window::handle_redo_state_update(bool canRedo)
{
  m_ui->action_redo->setEnabled(canRedo);
}

void window::handle_undo_text_update(const QString& text)
{
  m_ui->action_undo->setText("Undo " + text);
}

void window::handle_redo_text_update(const QString& text)
{
  m_ui->action_redo->setText("Redo " + text);
}

void window::handle_add_tileset(const model::tileset_info& info,
                                const QString& tabName) noexcept
{
  m_tilesetDock->get_tileset_widget()->add_tileset(info, tabName);
}

void window::handle_remove_tileset(int id) noexcept
{
  m_tilesetDock->get_tileset_widget()->remove_tileset(id);
}

void window::handle_center_camera(int mapWidth, int mapHeight)
{
  m_centralWidget->center_viewport(mapWidth, mapHeight);
  handle_draw();
}

void window::handle_move_camera(int dx, int dy)
{
  m_centralWidget->move_viewport(dx, dy);
  handle_draw();
}

void window::handle_draw()
{
  m_centralWidget->handle_trigger_redraw();
}

void window::handle_new_map(model::core_model* core, int id)
{
  m_centralWidget->add_new_map_tab(core, "map", id);
  m_centralWidget->select_tab(id);
  if (!in_editor_mode()) {
    enable_editor_view();
    show_all_docks();  // TODO just reopen docks that were visible
    emit request_center_camera();
  }
}

void window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);

  settings::set(cfg::window::last_layout_geometry(), saveGeometry());
  settings::set(cfg::window::last_layout_state(), saveState());
}

void window::handle_remove_tab(int tabID)
{
  emit request_close_map(tabID);

  // The tab isn't actually removed yet, this checks if there will be
  // no open tabs
  if (m_centralWidget->open_tabs() == 1) {
    enable_startup_view();
  }
}

void window::on_action_undo_triggered()
{
  if (in_editor_mode()) {
    emit request_undo();
  }
}

void window::on_action_redo_triggered()
{
  if (in_editor_mode()) {
    emit request_redo();
  }
}

void window::on_action_close_map_triggered()
{
  if (in_editor_mode()) {
    // TODO save current state of open map

    const auto id = m_centralWidget->active_tab_id();
    Q_ASSERT(id);

    m_centralWidget->close_tab(*id);
    emit request_close_map(*id);

    if (m_centralWidget->open_tabs() == 0) {
      enable_startup_view();
    }
  }
}

void window::on_action_tilesets_visibility_triggered()
{
  m_tilesetDock->setVisible(m_ui->action_tilesets_visibility->isChecked());
}

void window::on_action_mouse_tools_visibility_triggered()
{
  m_toolDock->setVisible(m_ui->action_mouse_tools_visibility->isChecked());
}

void window::on_action_save_triggered()
{
  // TODO
}

void window::on_action_save_as_triggered()
{
  // TODO
}

void window::on_action_rename_triggered()
{
  // TODO
}

void window::on_action_add_row_triggered()
{
  if (in_editor_mode()) {
    emit request_add_row();
  }
}

void window::on_action_add_column_triggered()
{
  if (in_editor_mode()) {
    emit request_add_col();
  }
}

void window::on_action_remove_row_triggered()
{
  if (in_editor_mode()) {
    emit request_remove_row();
  }
}

void window::on_action_remove_column_triggered()
{
  if (in_editor_mode()) {
    emit request_remove_col();
  }
}

void window::on_action_resize_map_triggered()
{
  if (in_editor_mode()) {
    emit request_resize_map();
  }
}

void window::on_action_toggle_grid_triggered()
{
  if (in_editor_mode()) {
    if (setting<bool> grid{cfg::graphics::grid()}; grid) {
      const auto prev = *grid;
      settings::set(cfg::graphics::grid(), !prev);
      handle_draw();
    }
  }
}

void window::on_action_pan_up_triggered()
{
  if (in_editor_mode()) {
    emit request_pan_up();
  }
}

void window::on_action_pan_down_triggered()
{
  if (in_editor_mode()) {
    emit request_pan_down();
  }
}

void window::on_action_pan_right_triggered()
{
  if (in_editor_mode()) {
    emit request_pan_right();
  }
}

void window::on_action_pan_left_triggered()
{
  if (in_editor_mode()) {
    emit request_pan_left();
  }
}

void window::on_action_zoom_in_triggered()
{
  if (in_editor_mode()) {
    emit request_increase_tile_size();
  }
}

void window::on_action_zoom_out_triggered()
{
  if (in_editor_mode()) {
    emit request_decrease_tile_size();
  }
}

void window::on_action_reset_zoom_triggered()
{
  if (in_editor_mode()) {
    emit request_reset_tile_size();
  }
}

void window::on_action_center_camera_triggered()
{
  if (in_editor_mode()) {
    emit request_center_camera();
  }
}

void window::on_action_reset_layout_triggered()
{
  reset_dock_layout();
}

void window::on_action_stamp_tool_triggered()
{
  m_toolDock->get_tool_widget()->handle_enable_stamp();
}

void window::on_action_bucket_tool_triggered()
{
  m_toolDock->get_tool_widget()->handle_enable_bucket();
}

void window::on_action_eraser_tool_triggered()
{
  m_toolDock->get_tool_widget()->handle_enable_eraser();
}

void window::on_action_rectangle_tool_triggered()
{
  m_toolDock->get_tool_widget()->handle_enable_rectangle();
}

void window::on_action_find_same_tool_triggered()
{
  m_toolDock->get_tool_widget()->handle_enable_find_same();
}

void window::on_action_exit_triggered()
{
  QApplication::exit();
}

void window::on_action_settings_triggered()
{
  settings_dialog settings;
  settings.exec();
}

void window::on_action_about_triggered()
{
  about_dialog about;
  about.exec();
}

}  // namespace tactile::gui
