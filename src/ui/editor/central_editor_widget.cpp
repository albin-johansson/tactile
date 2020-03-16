#include "central_editor_widget.h"

#include <QTabWidget>

#include "editor_tab.h"
#include "startup_widget.h"
#include "tile_map_tab_widget.h"
#include "ui_central_widget.h"

namespace tactile {

CentralEditorWidget::CentralEditorWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::CentralWidgetUI{}}
{
  m_ui->setupUi(this);

  m_mapTabWidget = new TileMapTabWidget{};

  m_startupViewIndex = m_ui->stackedWidget->addWidget(new StartupWidget{});
  m_editorViewIndex = m_ui->stackedWidget->addWidget(m_mapTabWidget);

  using TMTW = TileMapTabWidget;
  using CEW = CentralEditorWidget;
  connect(m_mapTabWidget, &TMTW::tmtw_req_redraw, this, &CEW::req_redraw);

  connect(
      m_mapTabWidget, &TMTW::tmtw_req_remove_tab, this, &CEW::ce_removed_tab);
}

CentralEditorWidget::~CentralEditorWidget() noexcept
{
  delete m_ui;
}

int CentralEditorWidget::add_new_map_tab(const QString& title) noexcept
{
  return m_mapTabWidget->add_tile_map_tab(title);
}

void CentralEditorWidget::close_tab(int id) noexcept
{
  m_mapTabWidget->remove_tile_map_tab(id);
}

void CentralEditorWidget::center_viewport(int mapWidth, int mapHeight) noexcept
{
  m_mapTabWidget->center_viewport(mapWidth, mapHeight);
}

void CentralEditorWidget::move_viewport(int dx, int dy) noexcept
{
  m_mapTabWidget->move_viewport(dx, dy);
}

void CentralEditorWidget::trigger_redraw() noexcept
{
  m_mapTabWidget->update();  // TODO check that this works
}

void CentralEditorWidget::enable_startup_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_startupViewIndex);
}

void CentralEditorWidget::enable_editor_view() noexcept
{
  m_ui->stackedWidget->setCurrentIndex(m_editorViewIndex);
}

bool CentralEditorWidget::in_editor_mode() const noexcept
{
  return m_ui->stackedWidget->currentIndex() == m_editorViewIndex;
}

Maybe<int> CentralEditorWidget::active_tab_id() const noexcept
{
  return m_mapTabWidget->active_tab_id();
}

int CentralEditorWidget::open_tabs() const noexcept
{
  return m_mapTabWidget->count();
}

}  // namespace tactile
