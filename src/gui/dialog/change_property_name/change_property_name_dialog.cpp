#include "change_property_name_dialog.hpp"

#include <QPushButton>

#include "init_ui.hpp"
#include "property_name_validator.hpp"
#include "ui_change_property_name_dialog.h"

namespace tactile {

ChangePropertyNameDialog::ChangePropertyNameDialog(QStandardItemModel* model,
                                                   QWidget* parent)
    : QDialog{parent}
    , mUi{InitUi<Ui::ChangePropertyNameDialog>(this)}
    , mValidator{new PropertyNameValidator{model, this}}
{
  // clang-format off
  connect(mUi->nameEdit, &QLineEdit::textChanged, this, &ChangePropertyNameDialog::OnNameChanged);
  // clang-format on

  mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ChangePropertyNameDialog::~ChangePropertyNameDialog() noexcept = default;

auto ChangePropertyNameDialog::Spawn(QStandardItemModel* model, QWidget* parent)
    -> maybe<QString>
{
  ChangePropertyNameDialog dialog{model, parent};
  if (dialog.exec())
  {
    return dialog.mUi->nameEdit->text();
  }
  else
  {
    return std::nullopt;
  }
}

void ChangePropertyNameDialog::OnNameChanged(const QString& name)
{
  auto result = name;
  auto unused = 0;
  const auto ok =
      mValidator->validate(result, unused) == QValidator::Acceptable;
  mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}

}  // namespace tactile
