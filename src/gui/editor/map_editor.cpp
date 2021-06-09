#include "map_editor.hpp"

#include "init_ui.hpp"
#include "map_document.hpp"
#include "map_editor_context_menu.hpp"
#include "map_tab_widget.hpp"
#include "startup_widget.hpp"
#include "ui_map_editor.h"

namespace tactile::gui {

map_editor::map_editor(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::map_editor>(this)}
    , m_tabWidget{new map_tab_widget{this}}
    , m_contextMenu{new map_editor_context_menu{this}}
    , m_startupID{m_ui->stackedWidget->addWidget(new startup_widget{this})}
    , m_editorID{m_ui->stackedWidget->addWidget(m_tabWidget)}
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setContentsMargins(0, 0, 0, 0);

  init_connections();
}

map_editor::~map_editor() noexcept = default;

void map_editor::init_connections()
{
  // clang-format off
  connect(this, &map_editor::theme_changed, m_tabWidget, &map_tab_widget::theme_changed);
  connect(m_tabWidget, &map_tab_widget::currentChanged, this, &map_editor::tab_changed);
  connect(m_tabWidget, &map_tab_widget::ui_remove_map, this, &map_editor::ui_remove_map);
  connect(m_tabWidget, &map_tab_widget::increase_zoom, this, &map_editor::increase_zoom);
  connect(m_tabWidget, &map_tab_widget::decrease_zoom, this, &map_editor::decrease_zoom);
  connect(m_tabWidget, &map_tab_widget::mouse_pressed, this, &map_editor::mouse_pressed);
  connect(m_tabWidget, &map_tab_widget::mouse_moved, this, &map_editor::mouse_moved);
  connect(m_tabWidget, &map_tab_widget::mouse_released, this, &map_editor::mouse_released);
  connect(m_tabWidget, &map_tab_widget::mouse_entered, this, &map_editor::mouse_entered);
  connect(m_tabWidget, &map_tab_widget::mouse_exited, this, &map_editor::mouse_exited);
  connect(m_tabWidget, &map_tab_widget::spawn_context_menu, this, &map_editor::spawn_context_menu);
  connect(m_contextMenu, &map_editor_context_menu::show_properties, this, &map_editor::show_map_properties);
  // clang-format on
}

void map_editor::enable_stamp_preview(const core::position& position)
{
  m_tabWidget->enable_stamp_preview(position);
}

void map_editor::disable_stamp_preview()
{
  m_tabWidget->disable_stamp_preview();
}

void map_editor::add_map_tab(core::map_document* map,
                             const map_id id,
                             const QString& title)
{
  m_tabWidget->add_map_tab(map, id, title);
}

void map_editor::select_tab(const map_id id)
{
  m_tabWidget->select_tab(id);
}

void map_editor::close_tab(const map_id id) noexcept
{
  m_tabWidget->remove_map_tab(id);
}

void map_editor::center_viewport()
{
  m_tabWidget->center_map();
}

void map_editor::move_map(const int dx, const int dy) noexcept
{
  m_tabWidget->move_map(dx, dy);
}

void map_editor::force_redraw()
{
  m_tabWidget->force_redraw();
}

void map_editor::enable_startup_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_startupID);
}

void map_editor::enable_editor_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_editorID);
}

void map_editor::set_active_tab_name(const QString& name)
{
  m_tabWidget->set_active_tab_name(name);
}

void map_editor::set_opengl_enabled(const bool enabled)
{
  m_tabWidget->set_opengl_enabled(enabled);
}

auto map_editor::in_editor_mode() const -> bool
{
  return m_ui->stackedWidget->currentIndex() == m_editorID;
}

auto map_editor::active_tab_id() const -> maybe<map_id>
{
  return m_tabWidget->active_tab_id();
}

auto map_editor::active_tab_name() const -> maybe<QString>
{
  return m_tabWidget->active_tab_name();
}

auto map_editor::tab_count() const -> int
{
  return m_tabWidget->count();
}

void map_editor::tab_changed(const int index)
{
  if (const auto id = m_tabWidget->id_from_index(index); id)
  {
    emit ui_select_map(*id);
  }
}

void map_editor::spawn_context_menu(const QPoint& pos)
{
  m_contextMenu->exec(pos);
}

void map_editor::show_map_properties()
{
  m_tabWidget->show_properties();
}

}  // namespace tactile::gui
