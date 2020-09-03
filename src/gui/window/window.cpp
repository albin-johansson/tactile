#include "window.hpp"

#include <qapplication.h>
#include <qdebug.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qlayoutitem.h>
#include <qopenglfunctions.h>

#include "about_dialog.hpp"
#include "map_editor.hpp"
#include "preferences.hpp"
#include "setting.hpp"
#include "settings_dialog.hpp"
#include "tool_widget.hpp"
#include "ui_window.h"

namespace tactile::gui {

window::window(QWidget* parent) : QMainWindow{parent}, m_ui{new Ui::window{}}
{
  m_ui->setupUi(this);

  // TODO add mini-map widget

  m_mainEditor = new map_editor{this};
  m_toolDock = new tool_dock{this};
  m_tilesetDock = new tileset_dock{this};

  setCentralWidget(m_mainEditor);
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

void window::init_connections()
{
  auto onTriggered = [this](auto&& action, auto&& fun) {
    connect(action, &QAction::triggered, this, fun);
  };

  onTriggered(m_ui->action_new_map, &window::request_new_map);
  onTriggered(m_ui->action_add_tileset, &window::request_new_tileset);

  connect(m_mainEditor,
          &map_editor::request_remove_tab,
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

  connect(m_mainEditor,
          &map_editor::request_select_tab,
          this,
          &window::request_select_map);

  connect(m_tilesetDock,
          &tileset_dock::new_tileset_requested,
          this,
          &window::request_new_tileset);
}

void window::enable_startup_view()
{
  m_mainEditor->enable_startup_view();
  m_toolDock->get_tool_widget()->disable_tools();
  hide_all_docks();
}

void window::enable_editor_view()
{
  m_mainEditor->enable_editor_view();

  // FIXME only enable relevant tools
  m_toolDock->get_tool_widget()->enable_tools();
}

void window::init_layout()
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
}

void window::hide_all_docks()
{
  m_toolDock->close();
  m_tilesetDock->close();
}

void window::show_all_docks()
{
  m_toolDock->show();
  m_tilesetDock->show();
}

auto window::in_editor_mode() const -> bool
{
  return m_mainEditor->in_editor_mode();
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

void window::handle_add_tileset(const QImage& image,
                                tileset_id id,
                                int tileWidth,
                                int tileHeight,
                                const QString& tabName)
{
  m_tilesetDock->get_tileset_widget()->add_tileset(
      image, id, tileWidth, tileHeight, tabName);
}

void window::handle_remove_tileset(tileset_id id)
{
  m_tilesetDock->get_tileset_widget()->remove_tileset(id);
}

void window::center_map()
{
  m_mainEditor->center_viewport();
  handle_draw();
}

void window::handle_move_camera(int dx, int dy)
{
  m_mainEditor->move_map(dx, dy);
  handle_draw();
}

void window::handle_draw()
{
  m_mainEditor->handle_redraw();
}

void window::handle_new_map(not_null<model::tilemap*> map, map_id id)
{
  m_mainEditor->add_new_map_tab(map, "map", id);  // TODO pass core and map_id?
  m_mainEditor->select_tab(id);
  if (!in_editor_mode()) {
    enable_editor_view();
    show_all_docks();  // TODO just reopen docks that were visible
    center_map();
  }
}

void window::closeEvent(QCloseEvent* event)
{
  QWidget::closeEvent(event);
  prefs::window::last_layout_geometry().set(saveGeometry());
  prefs::window::last_layout_state().set(saveState());
}

void window::handle_remove_tab(map_id tabID)
{
  emit request_close_map(tabID);

  // The tab isn't actually removed yet, this checks if there will be
  // no open tabs
  if (m_mainEditor->num_tabs() == 1) {
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

    const auto id = m_mainEditor->active_tab_id();
    Q_ASSERT(id);

    m_mainEditor->close_tab(*id);
    emit request_close_map(*id);

    if (m_mainEditor->num_tabs() == 0) {
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
    if (auto grid = prefs::graphics::render_grid(); grid) {
      grid.set(!*grid);
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
    center_map();
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

void window::on_action_settings_triggered()
{
  settings_dialog settings;
  connect(&settings,
          &settings_dialog::reload_theme,
          this,
          &window::handle_theme_changed);

  settings.exec();
}

void window::on_action_about_qt_triggered()
{
  QApplication::aboutQt();
}

void window::on_action_exit_triggered()
{
  QApplication::exit();
}

void window::on_action_about_triggered()
{
  about_dialog about;
  about.exec();
}

void window::handle_theme_changed()
{
  emit m_mainEditor->theme_changed();
}

}  // namespace tactile::gui
