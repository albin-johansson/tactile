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
  connect(m_ui->actionAbout_Tactile,
          SIGNAL(triggered()),
          this,
          SLOT(display_about_dialog()));

  connect(m_ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

  connect(m_ui->actionSettings,
          SIGNAL(triggered()),
          this,
          SLOT(display_settings_dialog()));

  connect(m_editorPane,
          SIGNAL(received_paint_event()),
          this,
          SLOT(redraw()));
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
  emit render(painter);
}

}  // namespace tactile
