#include "startup_widget.hpp"

#include "init_ui.hpp"
#include "ui_startup_widget.h"

namespace tactile::gui {

startup_widget::startup_widget(QWidget* parent)
    : QWidget{parent}
    , m_ui{init_ui<Ui::startup_widget>(this)}
{
  const QKeySequence newDocument{QKeySequence::New};
  m_ui->newShortcutLabel->setText(newDocument.toString());

  const QKeySequence openDocument{QKeySequence::Open};
  m_ui->openShortcutLabel->setText(openDocument.toString());
}

startup_widget::~startup_widget() noexcept = default;

}  // namespace tactile::gui
