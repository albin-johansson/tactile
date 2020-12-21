#include "file_value_widget.hpp"

#include "ui_file_value_widget.h"

namespace tactile::gui {

file_value_widget::file_value_widget(QWidget* parent)
    : property_value_widget{parent}
    , m_ui{new Ui::file_value_widget{}}
{
  m_ui->setupUi(this);

  setAutoFillBackground(true);
  layout()->setContentsMargins(0, 0, 0, 0);

  // clang-format off
  connect(m_ui->selectFileButton, &QToolButton::pressed,
          this,                   &file_value_widget::spawn_dialog);
  // clang-format on

  // This is to avoid the idle page preventing the active page from being
  // activated after being pressed
  m_ui->idlePage->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

file_value_widget::~file_value_widget() noexcept
{
  delete m_ui;
}

void file_value_widget::enter_active_mode()
{
  m_ui->stackedWidget->setCurrentWidget(m_ui->activePage);
}

void file_value_widget::enter_idle_mode()
{
  m_ui->stackedWidget->setCurrentWidget(m_ui->idlePage);
}

void file_value_widget::reset_path()
{
  m_ui->activePathEdit->clear();
}

void file_value_widget::set_path(const QFileInfo& path)
{
  m_ui->activePathEdit->setText(path.filePath());
  m_ui->idlePathEdit->setText(path.fileName());
}

auto file_value_widget::current_path() const -> QFileInfo
{
  return m_ui->activePathEdit->text();
}

}  // namespace tactile::gui
