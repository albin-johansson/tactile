#include "main_editor_widget.h"

#include <QHBoxLayout>

#include "editor_pane.h"
#include "mouse_tool_widget.h"
#include "ui_main_editor_widget.h"

namespace tactile {

MainEditorWidget::MainEditorWidget(QWidget* parent)
    : QWidget{parent}, m_ui{new Ui::MainEditorWidgetUI{}}
{
  m_ui->setupUi(this);

  m_editorPane = new EditorPane{};

  auto* layout = new QHBoxLayout{};
  layout->addWidget(new MouseToolWidget{});
  layout->addWidget(m_editorPane);

  m_ui->gridLayout->addLayout(layout, 0, 0);
}

MainEditorWidget::~MainEditorWidget() noexcept
{
  delete m_ui;
}

}  // namespace tactile
