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
#include "save_as_dialog.hpp"
#include "save_service.hpp"
#include "setting.hpp"
#include "settings_dialog.hpp"
#include "tool_widget.hpp"
#include "ui_window.h"

namespace tactile::gui {

window::window(QWidget* parent) : QMainWindow{parent}, m_ui{new Ui::window{}}
{
  m_ui->setupUi(this);

  // TODO add mini-map widget

  m_editor = new map_editor{this};
  m_toolDock = new tool_dock{this};
  m_tilesetDock = new tileset_dock{this};

  setCentralWidget(m_editor);
  addDockWidget(Qt::LeftDockWidgetArea, m_toolDock);
  addDockWidget(Qt::RightDockWidgetArea, m_tilesetDock);

  init_connections();
  init_layout();
  enter_no_content_view();  // TODO option to reopen last map
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

  connect(m_editor,
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

    connect(m_toolDock,
            &tool_dock::stamp_enabled,
            this,
            &window::handle_stamp_enabled);
    connect(m_toolDock,
            &tool_dock::bucket_enabled,
            this,
            &window::handle_bucket_enabled);
    connect(m_toolDock,
            &tool_dock::eraser_enabled,
            this,
            &window::handle_eraser_enabled);
  }

  connect(m_toolDock,
          &QDockWidget::visibilityChanged,
          m_ui->action_mouse_tools_visibility,
          &QAction::setChecked);

  connect(m_tilesetDock,
          &QDockWidget::visibilityChanged,
          m_ui->action_tilesets_visibility,
          &QAction::setChecked);

  connect(m_editor,
          &map_editor::request_select_tab,
          this,
          &window::request_select_map);

  connect(m_editor, &map_editor::mouse_pressed, this, &window::mouse_pressed);
  connect(m_editor, &map_editor::mouse_moved, this, &window::mouse_moved);
  connect(m_editor, &map_editor::mouse_released, this, &window::mouse_released);
  connect(m_editor, &map_editor::mouse_entered, this, &window::mouse_entered);
  connect(m_editor, &map_editor::mouse_exited, this, &window::mouse_exited);

  connect(m_tilesetDock,
          &tileset_dock::new_tileset_requested,
          this,
          &window::request_new_tileset);

  connect(m_tilesetDock,
          &tileset_dock::selected_tileset,
          this,
          &window::selected_tileset);

  connect(m_tilesetDock,
          &tileset_dock::removed_tileset,
          this,
          &window::removed_tileset);

  connect(m_tilesetDock,
          &tileset_dock::tileset_selection_changed,
          this,
          &window::tileset_selection_changed);
}

void window::enter_no_content_view()
{
  m_editor->enable_startup_view();
  m_toolDock->get_tool_widget()->disable_tools();

  set_actions_enabled(false);
  hide_all_docks();
}

void window::enter_content_view()
{
  m_editor->enable_editor_view();
  m_toolDock->get_tool_widget()->enable_tools();

  set_actions_enabled(true);
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
  return m_editor->in_editor_mode();
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
  m_ui->action_undo->setText(QStringLiteral(u"Undo ") + text);
}

void window::handle_redo_text_update(const QString& text)
{
  m_ui->action_redo->setText(QStringLiteral(u"Redo ") + text);
}

void window::handle_add_tileset(const QImage& image,
                                tileset_id id,
                                tile_width tileWidth,
                                tile_height tileHeight,
                                const QString& tabName)
{
  m_tilesetDock->get_tileset_widget()->add_tileset(
      image, id, tileWidth, tileHeight, tabName);
}

void window::center_map()
{
  m_editor->center_viewport();
  handle_draw();
}

void window::set_actions_enabled(bool enabled)
{
  // File
  m_ui->action_close_map->setEnabled(enabled);
  m_ui->action_save->setEnabled(enabled);
  m_ui->action_save_as->setEnabled(enabled);
  m_ui->action_rename->setEnabled(enabled);

  // Edit
  m_ui->action_add_column->setEnabled(enabled);
  m_ui->action_add_row->setEnabled(enabled);
  m_ui->action_remove_row->setEnabled(enabled);
  m_ui->action_remove_column->setEnabled(enabled);
  m_ui->action_resize_map->setEnabled(enabled);
  m_ui->action_stamp_tool->setEnabled(enabled);
  m_ui->action_bucket_tool->setEnabled(enabled);
  m_ui->action_eraser_tool->setEnabled(enabled);

  // View
  m_ui->action_center_camera->setEnabled(enabled);
  m_ui->action_toggle_grid->setEnabled(enabled);
  m_ui->action_zoom_in->setEnabled(enabled);
  m_ui->action_zoom_out->setEnabled(enabled);
  m_ui->action_reset_zoom->setEnabled(enabled);
  m_ui->action_pan_up->setEnabled(enabled);
  m_ui->action_pan_down->setEnabled(enabled);
  m_ui->action_pan_right->setEnabled(enabled);
  m_ui->action_pan_left->setEnabled(enabled);
}

void window::handle_move_camera(int dx, int dy)
{
  m_editor->move_map(dx, dy);
  handle_draw();
}

void window::handle_draw()
{
  m_editor->handle_redraw();
}

void window::enable_stamp_preview(const core::position& position)
{
  m_editor->enable_stamp_preview(position);
}

void window::disable_stamp_preview()
{
  m_editor->disable_stamp_preview();
}

void window::handle_new_map(not_null<core::map*> map,
                            not_null<core::tileset_manager*> tilesets,
                            map_id id)
{
  m_editor->add_new_map_tab(
      map, tilesets, "map", id);  // TODO pass core and map_id?
  m_editor->select_tab(id);
  if (!in_editor_mode()) {
    enter_content_view();
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
  if (m_editor->num_tabs() == 1) {
    enter_no_content_view();
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

    const auto id = m_editor->active_tab_id();
    Q_ASSERT(id);

    m_editor->close_tab(*id);
    emit request_close_map(*id);

    if (m_editor->num_tabs() == 0) {
      enter_no_content_view();
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
  auto get_tab_name = [this]() -> QString {
    if (const auto name = m_editor->active_tab_name(); name) {
      return *name;
    } else {
      return QStringLiteral(u"map");
    }
  };
  save_as_dialog::spawn([this](const QString& path) { emit save_as(path); },
                        get_tab_name());
}

void window::on_action_rename_triggered()
{
  // TODO
}

void window::on_action_add_row_triggered()
{
  if (in_editor_mode()) {  // FIXME these are redundant now?
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
  emit m_editor->theme_changed();
}

void window::handle_stamp_enabled()
{
  m_ui->action_stamp_tool->setChecked(true);
  emit select_tool(tool_id::stamp);
}

void window::handle_bucket_enabled()
{
  m_ui->action_bucket_tool->setChecked(true);
  emit select_tool(tool_id::bucket);
}

void window::handle_eraser_enabled()
{
  m_ui->action_eraser_tool->setChecked(true);
  emit select_tool(tool_id::eraser);
}

void window::handle_rectangle_enabled()
{
  m_ui->action_rectangle_tool->setChecked(true);
  emit select_tool(tool_id::rectangle);
}

void window::handle_find_same_enabled()
{
  m_ui->action_find_same_tool->setChecked(true);
  emit select_tool(tool_id::find_same);
}

}  // namespace tactile::gui
