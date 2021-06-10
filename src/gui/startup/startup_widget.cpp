#include "startup_widget.hpp"

#include "init_ui.hpp"
#include "ui_startup_widget.h"

namespace tactile {

StartupWidget::StartupWidget(QWidget* parent)
    : QWidget{parent}
    , mUi{InitUi<Ui::StartupWidget>(this)}
{
  const QKeySequence newDocument{QKeySequence::New};
  mUi->newShortcutLabel->setText(newDocument.toString());

  const QKeySequence openDocument{QKeySequence::Open};
  mUi->openShortcutLabel->setText(openDocument.toString());
}

StartupWidget::~StartupWidget() noexcept = default;

}  // namespace tactile
