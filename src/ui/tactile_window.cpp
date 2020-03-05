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
  using TW = TactileWindow;
  using EP = EditorPane;

  const auto on_triggered = [this](QAction* action, auto fun) noexcept {
    connect(action, &QAction::triggered, this, fun);
  };

  on_triggered(m_ui->actionAbout_Tactile, &TW::display_about_dialog);
  on_triggered(m_ui->actionExit, &TW::exit);
  on_triggered(m_ui->actionSettings, &TW::display_settings_dialog);
  on_triggered(m_ui->actionAdd_row, [this] { emit add_row(); });
  on_triggered(m_ui->actionAdd_column, [this] { emit add_col(); });
  on_triggered(m_ui->actionRemove_row, [this] { emit remove_row(); });
  on_triggered(m_ui->actionRemove_column, [this] { emit remove_col(); });

  connect(m_editorPane, &EP::received_paint_event, this, &TW::redraw);
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

void TactileWindow::trigger_redraw()
{
  m_editorPane->update();
}

}  // namespace tactile
