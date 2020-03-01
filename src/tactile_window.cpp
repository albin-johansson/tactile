#include "tactile_window.h"

#include <QOpenGLFunctions>
#include <QPainter>
#include <iostream>

#include "editor_pane.h"
#include "ui_window.h"

namespace tactile {

TactileWindow::TactileWindow(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow{}}
{
  m_ui->setupUi(this);

//  m_editorPane = new EditorPane{};
//  m_ui->gridLayout->addWidget(m_editorPane);setCe
//  setCentralWidget(m_editorPane);
}

TactileWindow::~TactileWindow()
{
  delete m_ui;
//  delete m_editorPane;
}

}  // namespace tactile
