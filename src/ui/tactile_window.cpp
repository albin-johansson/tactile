#include "tactile_window.h"

#include <QOpenGLFunctions>
#include <QPainter>

#include "editor_pane.h"
#include "about_dialog.h"
#include "ui_window.h"

namespace tactile {

TactileWindow::TactileWindow(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow{}}
{
  m_ui->setupUi(this);

  m_editorPane = new EditorPane{};

  connect(m_ui->actionAbout_Tactile,
          SIGNAL(triggered()),
          this,
          SLOT(display_about_dialog()));
}

TactileWindow::~TactileWindow()
{
  delete m_ui;
  delete m_editorPane;
}

void TactileWindow::display_about_dialog() noexcept
{
  AboutDialog about;
  about.exec();
}

}  // namespace tactile
