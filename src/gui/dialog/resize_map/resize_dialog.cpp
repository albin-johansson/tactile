#include "resize_dialog.hpp"

#include "init_ui.hpp"
#include "ui_resize_dialog.h"

namespace tactile {

ResizeDialog::ResizeDialog(QWidget* parent)
    : QDialog{parent}
    , mUi{InitUi<Ui::ResizeDialog>(this)}
    , mValidator{new QIntValidator{1, 20'000, this}}
{
  mUi->widthEdit->setValidator(mValidator);
  mUi->heightEdit->setValidator(mValidator);

  ConnectLineEdit(mUi->widthEdit);
  ConnectLineEdit(mUi->heightEdit);

  OkButton()->setEnabled(false);
}

ResizeDialog::~ResizeDialog() noexcept = default;

void ResizeDialog::ConnectLineEdit(QLineEdit* edit)
{
  if (edit)
  {
    connect(edit, &QLineEdit::textChanged, this, &ResizeDialog::ValidateInput);
  }
}

void ResizeDialog::ValidateInput()
{
  const auto widthState = IsValid(*mUi->widthEdit);
  const auto heightState = IsValid(*mUi->heightEdit);
  OkButton()->setEnabled(widthState == QValidator::Acceptable &&
                         heightState == QValidator::Acceptable);
  if (OkButton()->isEnabled())
  {
    bool ok{};

    if (const auto width = mUi->widthEdit->displayText().toInt(&ok); ok)
    {
      mChosenWidth = col_t{width};
    }

    if (const auto height = mUi->heightEdit->displayText().toInt(&ok); ok)
    {
      mChosenHeight = row_t{height};
    }
  }
}

auto ResizeDialog::OkButton() const -> QPushButton*
{
  return mUi->buttonBox->button(QDialogButtonBox::Ok);
}

auto ResizeDialog::IsValid(const QLineEdit& edit) const -> QValidator::State
{
  auto unused = 0;
  auto txt = edit.displayText();
  return mValidator->validate(txt, unused);
}

}  // namespace tactile
