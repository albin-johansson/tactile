#include "central_editor_widget.h"

#include "render_pane.h"
#include "startup_widget.h"
#include "ui_central_widget.h"

namespace tactile {

CentralEditorWidget::CentralEditorWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::CentralWidgetUI{}}
{
  m_ui->setupUi(this);

  m_renderPane = new RenderPane{};

  m_startupViewIndex = m_ui->stackedWidget->addWidget(new StartupWidget{});
  m_editorViewIndex = m_ui->stackedWidget->addWidget(m_renderPane);

  {
    using RP = RenderPane;
    using CW = CentralEditorWidget;
    connect(m_renderPane, &RP::rp_req_redraw, this, &CW::req_redraw);
  }
}

CentralEditorWidget::~CentralEditorWidget() noexcept
{
  delete m_ui;
}

void CentralEditorWidget::center_viewport(int mapWidth, int mapHeight) noexcept
{
  m_renderPane->center_viewport(mapWidth, mapHeight);
}

void CentralEditorWidget::trigger_redraw() noexcept
{
  m_renderPane->update();
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

const QRect& CentralEditorWidget::current_viewport() const noexcept
{
  return m_renderPane->viewport();
}

// QWidget* CentralWidget::render_widget() noexcept
//{
//  return m_renderPane;
//}

}  // namespace tactile
