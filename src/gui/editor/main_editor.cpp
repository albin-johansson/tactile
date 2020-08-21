#include "main_editor.hpp"

#include <QTabWidget>

#include "startup_widget.hpp"
#include "tilemap_tab.hpp"
#include "ui_main_editor.h"

namespace tactile::gui {

main_editor::main_editor(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::main_editor{}}
{
  m_ui->setupUi(this);

  m_mapTabWidget = new tilemap_tab{this};

  m_startupID = m_ui->stackedWidget->addWidget(new startup_widget{this});
  m_editorID = m_ui->stackedWidget->addWidget(m_mapTabWidget);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  init_connections();
}

main_editor::~main_editor() noexcept
{
  delete m_ui;
}

void main_editor::init_connections() noexcept
{
  connect(this,
          &main_editor::theme_changed,
          m_mapTabWidget,
          &tilemap_tab::theme_changed);

  connect(m_mapTabWidget,
          &tilemap_tab::request_remove_tab,
          this,
          &main_editor::request_remove_tab);

  connect(m_mapTabWidget,
          &tilemap_tab::currentChanged,
          this,
          &main_editor::tab_changed);
}

void main_editor::add_new_map_tab(not_null<model::tilemap*> map,
                                  const QString& title,
                                  int id) noexcept
{
  m_mapTabWidget->add_tile_map_tab(map, title, id);
}

void main_editor::select_tab(int id)
{
  m_mapTabWidget->select_tab(id);
}

void main_editor::close_tab(int id) noexcept
{
  m_mapTabWidget->remove_tile_map_tab(id);
}

void main_editor::center_viewport(int mapWidth, int mapHeight) noexcept
{
  m_mapTabWidget->center_viewport(mapWidth, mapHeight);
}

void main_editor::move_viewport(int dx, int dy) noexcept
{
  m_mapTabWidget->move_viewport(dx, dy);
}

void main_editor::handle_redraw()
{
  m_mapTabWidget->redraw();
}

void main_editor::enable_startup_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_startupID);
}

void main_editor::enable_editor_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_editorID);
}

auto main_editor::in_editor_mode() const noexcept -> bool
{
  return m_ui->stackedWidget->currentIndex() == m_editorID;
}

auto main_editor::active_tab_id() const noexcept -> std::optional<int>
{
  return m_mapTabWidget->active_tab_id();
}

auto main_editor::open_tabs() const noexcept -> int
{
  return m_mapTabWidget->count();
}

void main_editor::tab_changed(int index)
{
  if (const auto id = m_mapTabWidget->tab_id(index); id) {
    emit request_select_tab(*id);
  }
}

}  // namespace tactile::gui
