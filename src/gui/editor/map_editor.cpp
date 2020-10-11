#include "map_editor.hpp"

#include <qtabwidget.h>

#include "map_tab_widget.hpp"
#include "startup_widget.hpp"
#include "ui_map_editor.h"

namespace tactile::gui {

map_editor::map_editor(QWidget* parent)
    : QWidget{parent},
      m_ui{new Ui::map_editor{}}
{
  m_ui->setupUi(this);

  m_mapTabWidget = new map_tab_widget{this};

  m_startupID = m_ui->stackedWidget->addWidget(new startup_widget{this});
  m_editorID = m_ui->stackedWidget->addWidget(m_mapTabWidget);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  init_connections();
}

map_editor::~map_editor() noexcept
{
  delete m_ui;
}

void map_editor::init_connections()
{
  connect(this,
          &map_editor::theme_changed,
          m_mapTabWidget,
          &map_tab_widget::theme_changed);

  connect(m_mapTabWidget,
          &map_tab_widget::request_remove_tab,
          this,
          &map_editor::request_remove_tab);

  connect(m_mapTabWidget,
          &map_tab_widget::currentChanged,
          this,
          &map_editor::tab_changed);

  connect(m_mapTabWidget,
          &map_tab_widget::mouse_pressed,
          this,
          &map_editor::mouse_pressed);

  connect(m_mapTabWidget,
          &map_tab_widget::mouse_moved,
          this,
          &map_editor::mouse_moved);

  connect(m_mapTabWidget,
          &map_tab_widget::mouse_released,
          this,
          &map_editor::mouse_released);

  connect(m_mapTabWidget,
          &map_tab_widget::mouse_entered,
          this,
          &map_editor::mouse_entered);

  connect(m_mapTabWidget,
          &map_tab_widget::mouse_exited,
          this,
          &map_editor::mouse_exited);
}

void map_editor::enable_stamp_preview(const core::position& position)
{
  m_mapTabWidget->enable_stamp_preview(position);
}

void map_editor::disable_stamp_preview()
{
  m_mapTabWidget->disable_stamp_preview();
}

void map_editor::add_new_map_tab(core::map_document* map,
                                 map_id id,
                                 const QString& title)
{
  m_mapTabWidget->add_map_tab(map, id, title);
}

void map_editor::select_tab(map_id id)
{
  m_mapTabWidget->select_tab(id);
}

void map_editor::close_tab(map_id id) noexcept
{
  m_mapTabWidget->remove_map_tab(id);
}

void map_editor::center_viewport()
{
  m_mapTabWidget->center_map();
}

void map_editor::move_map(int dx, int dy) noexcept
{
  m_mapTabWidget->move_map(dx, dy);
}

void map_editor::handle_redraw()
{
  m_mapTabWidget->redraw();
}

void map_editor::enable_startup_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_startupID);
}

void map_editor::enable_editor_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_editorID);
}

auto map_editor::in_editor_mode() const -> bool
{
  return m_ui->stackedWidget->currentIndex() == m_editorID;
}

auto map_editor::active_tab_id() const -> std::optional<map_id>
{
  return m_mapTabWidget->active_tab_id();
}

auto map_editor::active_tab_name() const -> std::optional<QString>
{
  return m_mapTabWidget->active_tab_name();
}

auto map_editor::num_tabs() const -> int
{
  return m_mapTabWidget->count();
}

void map_editor::tab_changed(int index)
{
  if (const auto id = m_mapTabWidget->id_from_index(index); id) {
    emit request_select_tab(*id);
  }
}

}  // namespace tactile::gui
