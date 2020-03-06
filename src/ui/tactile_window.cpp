#include "tactile_window.h"

#include <QApplication>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QSpacerItem>

#include "about_dialog.h"
#include "editor_pane.h"
#include "mouse_tool_widget.h"
#include "settings_dialog.h"
#include "ui_window.h"

namespace tactile {

TactileWindow::TactileWindow(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow{}}
{
  m_ui->setupUi(this);

  m_editorPane = new EditorPane{this};
  m_mouseToolWidget = new MouseToolWidget{this};

  {
    auto* layout = m_ui->mainLayout;
    layout->addWidget(m_mouseToolWidget);
    layout->addWidget(m_editorPane);
    layout->update();
  }

  init_connections();
}

TactileWindow::~TactileWindow() noexcept
{
  delete m_mouseToolWidget;
  delete m_editorPane;
  delete m_ui;
}

void TactileWindow::init_connections() noexcept
{
  using W = TactileWindow;
  using EP = EditorPane;

  const auto on_triggered = [this](QAction* action, auto fun) noexcept {
    connect(action, &QAction::triggered, this, fun);
  };

  on_triggered(m_ui->actionAboutTactile, &W::display_about_dialog);
  on_triggered(m_ui->actionExit, &W::exit);
  on_triggered(m_ui->actionSettings, &W::display_settings_dialog);
  on_triggered(m_ui->actionAddRow, [this] { emit req_add_row(); });
  on_triggered(m_ui->actionAddColumn, [this] { emit req_add_col(); });
  on_triggered(m_ui->actionRemoveRow, [this] { emit req_remove_row(); });
  on_triggered(m_ui->actionRemoveColumn, [this] { emit req_remove_col(); });

  on_triggered(m_ui->actionPanUp, [this] {
    m_editorPane->move_viewport(0, -20);  // FIXME
    trigger_redraw();
  });

  on_triggered(m_ui->actionPanDown, [this] {
    m_editorPane->move_viewport(0, 20);  // FIXME
    trigger_redraw();
  });

  on_triggered(m_ui->actionPanRight, [this] {
    m_editorPane->move_viewport(20, 0);  // FIXME
    trigger_redraw();
  });

  on_triggered(m_ui->actionPanLeft, [this] {
    m_editorPane->move_viewport(-20, 0);  // FIXME
    trigger_redraw();
  });

  on_triggered(m_ui->actionCenterCamera, [this] { emit req_center_camera(); });

  connect(m_editorPane, &EP::req_redraw, this, &W::redraw);
}

void TactileWindow::display_about_dialog() noexcept
{
  AboutDialog about;
  about.exec();
}

void TactileWindow::display_settings_dialog() noexcept
{
  SettingsDialog settings;
  settings.exec();
}

void TactileWindow::exit() noexcept
{
  QApplication::exit();
}

void TactileWindow::redraw()
{
  QPainter painter{m_editorPane};
  painter.fillRect(0, 0, width(), height(), Qt::black);
  painter.setViewport(m_editorPane->viewport());
  emit req_render(painter);
}

void TactileWindow::center_camera(int mapWidth, int mapHeight)
{
  const auto& viewport = m_editorPane->viewport();
  const auto x = (viewport.width() - mapWidth) / 2;
  const auto y = (viewport.height() - mapHeight) / 2;

  m_editorPane->set_viewport_pos(x, y);
  trigger_redraw();
}

void TactileWindow::trigger_redraw()
{
  m_editorPane->update();
}

void TactileWindow::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  static bool first = true;
  if (first) {
    emit req_center_camera();
    first = false;
  }
}

}  // namespace tactile
