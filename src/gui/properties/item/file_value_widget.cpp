#include "file_value_widget.hpp"

#include "ui_file_value_widget.h"

namespace tactile::gui {

file_value_widget::file_value_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{new Ui::file_value_widget{}}
{
  m_ui->setupUi(this);

  setAutoFillBackground(true);
  layout()->setContentsMargins(0, 0, 0, 0);

  // clang-format off
  connect(m_ui->selectFileButton, &QToolButton::pressed,
          this,                   &file_value_widget::spawn_dialog);
  // clang-format on
}

void file_value_widget::set_visible(bool visible)
{
  QWidget::setVisible(visible);
}

void file_value_widget::set_path(const QFileInfo& path)
{
  m_ui->pathEdit->setText(path.filePath());
}

}  // namespace tactile::gui
