#include "central_editor_widget.hpp"

#include <QTabWidget>

#include "editor_tab.hpp"
#include "tile_map_tab_widget.hpp"
#include "ui/startup/startup_widget.hpp"
#include "ui_central_widget.h"
#include "widget_size_policy.hpp"

namespace tactile::ui {

central_editor_widget::central_editor_widget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::CentralWidgetUI{}}
{
  m_ui->setupUi(this);

  m_mapTabWidget = new tilemap_tab_widget{};

  m_startupViewIndex = m_ui->stackedWidget->addWidget(new startup_widget{});
  m_editorViewIndex = m_ui->stackedWidget->addWidget(m_mapTabWidget);

  init_connections();
  set_size_policy(this, QSizePolicy::Expanding);
}

central_editor_widget::~central_editor_widget() noexcept
{
  delete m_ui;
}

void central_editor_widget::init_connections() noexcept
{
  using TMTW = tilemap_tab_widget;
  using CEW = central_editor_widget;
  connect(m_mapTabWidget, &TMTW::s_redraw, this, &CEW::s_redraw);
  connect(m_mapTabWidget, &TMTW::s_remove_tab, this, &CEW::s_removed_tab);
  connect(m_mapTabWidget, &TMTW::currentChanged, this, [this](int index) {
    if (const auto id = m_mapTabWidget->tab_id(index); id) {
      emit s_selected_tab(*id);
    }
  });
}

int central_editor_widget::add_new_map_tab(const QString& title) noexcept
{
  return m_mapTabWidget->add_tile_map_tab(title);
}

void central_editor_widget::close_tab(int id) noexcept
{
  m_mapTabWidget->remove_tile_map_tab(id);
}

void central_editor_widget::center_viewport(int mapWidth, int mapHeight) noexcept
{
  m_mapTabWidget->center_viewport(mapWidth, mapHeight);
}

void central_editor_widget::move_viewport(int dx, int dy) noexcept
{
  m_mapTabWidget->move_viewport(dx, dy);
}

void central_editor_widget::handle_trigger_redraw() noexcept
{
  m_mapTabWidget->update();
}

void central_editor_widget::enable_startup_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_startupViewIndex);
}

void central_editor_widget::enable_editor_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_editorViewIndex);
}

bool central_editor_widget::in_editor_mode() const noexcept
{
  return m_ui->stackedWidget->currentIndex() == m_editorViewIndex;
}

std::optional<int> central_editor_widget::active_tab_id() const noexcept
{
  return m_mapTabWidget->active_tab_id();
}

int central_editor_widget::open_tabs() const noexcept
{
  return m_mapTabWidget->count();
}

}  // namespace tactile::ui
