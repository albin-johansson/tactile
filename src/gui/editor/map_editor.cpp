#include "map_editor.hpp"

#include <QTabWidget>

#include "map_tab_widget.hpp"
#include "startup_widget.hpp"
#include "ui_map_editor.h"

namespace tactile::gui {

map_editor::map_editor(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::map_editor{}}
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

void map_editor::init_connections() noexcept
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
}

void map_editor::add_new_map_tab(not_null<model::tilemap*> map,
                                 const QString& title,
                                 map_id id) noexcept
{
  m_mapTabWidget->add_map_tab(map, title, id);
}

void map_editor::select_tab(map_id id)
{
  m_mapTabWidget->select_tab(id);
}

void map_editor::close_tab(map_id id) noexcept
{
  m_mapTabWidget->remove_map_tab(id);
}

void map_editor::center_viewport(int mapWidth, int mapHeight) noexcept
{
  m_mapTabWidget->center_viewport(mapWidth, mapHeight);
}

void map_editor::move_viewport(int dx, int dy) noexcept
{
  m_mapTabWidget->move_viewport(dx, dy);
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

auto map_editor::in_editor_mode() const noexcept -> bool
{
  return m_ui->stackedWidget->currentIndex() == m_editorID;
}

auto map_editor::active_tab_id() const noexcept -> std::optional<map_id>
{
  return m_mapTabWidget->active_tab_id();
}

auto map_editor::open_tabs() const noexcept -> int
{
  return m_mapTabWidget->count();
}

void map_editor::tab_changed(int index)
{
  if (const auto id = m_mapTabWidget->tab_id(index); id) {
    emit request_select_tab(*id);
  }
}

}  // namespace tactile::gui
