#include "file_value_widget.hpp"

#include "init_ui.hpp"
#include "ui_file_value_widget.h"

namespace tactile {

FileValueWidget::FileValueWidget(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::file_value_widget>(this)}
{
  setAutoFillBackground(true);
  layout()->setContentsMargins(0, 0, 0, 0);

  // clang-format off
  connect(mUi->selectFileButton, &QToolButton::pressed,
          this, &FileValueWidget::S_SpawnDialog);
  // clang-format on

  /* This is to avoid the idle page preventing the active page from being
     activated after being pressed */
  mUi->idlePage->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

FileValueWidget::~FileValueWidget() noexcept = default;

void FileValueWidget::EnterActiveMode()
{
  mUi->stackedWidget->setCurrentWidget(mUi->activePage);
}

void FileValueWidget::EnterIdleMode()
{
  mUi->stackedWidget->setCurrentWidget(mUi->idlePage);
}

void FileValueWidget::ResetPath()
{
  mUi->activePathEdit->clear();
}

void FileValueWidget::SetPath(const QString& path)
{
  const QFileInfo info{path};
  mUi->activePathEdit->setText(info.filePath());
  mUi->idlePathEdit->setText(info.fileName());
}

auto FileValueWidget::CurrentPath() const -> QString
{
  return mUi->activePathEdit->text();
}

}  // namespace tactile
