#include "add_property_dialog.hpp"

#include <QPushButton>  // QPushButton

#include "init_ui.hpp"
#include "property_name_validator.hpp"
#include "tactile_error.hpp"
#include "ui_add_property_dialog.h"

namespace tactile {

AddPropertyDialog::AddPropertyDialog(QStandardItemModel* model, QWidget* parent)
    : QDialog{parent}
    , mUi{InitUi<Ui::AddPropertyDialog>(this)}
    , mNameValidator{new PropertyNameValidator{model, this}}
{
  setObjectName(QStringLiteral(u"AddPropertyDialog"));

  // clang-format off
  connect(mUi->nameEdit, &QLineEdit::textChanged, this, &AddPropertyDialog::OnNameChanged);
  connect(mUi->typeComboBox, &QComboBox::currentTextChanged, this, &AddPropertyDialog::OnTypeChanged);
  // clang-format on

  UpdateType();
  mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

AddPropertyDialog::~AddPropertyDialog() noexcept = default;

void AddPropertyDialog::UpdateType()
{
  const auto text = mUi->typeComboBox->currentText();
  if (text == QStringLiteral(u"string"))
  {
    mType = core::PropertyType::String;
  }
  else if (text == QStringLiteral(u"int"))
  {
    mType = core::PropertyType::Integer;
  }
  else if (text == QStringLiteral(u"float"))
  {
    mType = core::PropertyType::Floating;
  }
  else if (text == QStringLiteral(u"bool"))
  {
    mType = core::PropertyType::Boolean;
  }
  else if (text == QStringLiteral(u"color"))
  {
    mType = core::PropertyType::Color;
  }
  else if (text == QStringLiteral(u"object"))
  {
    mType = core::PropertyType::Object;
  }
  else if (text == QStringLiteral(u"file"))
  {
    mType = core::PropertyType::File;
  }
  else
  {
    throw TactileError{"Did not recognize property type!"};
  }
}

void AddPropertyDialog::OnNameChanged(const QString& name)
{
  auto result = name;
  auto unused = 0;
  const auto isOk =
      mNameValidator->validate(result, unused) == QValidator::Acceptable;

  auto* okButton = mUi->buttonBox->button(QDialogButtonBox::Ok);
  okButton->setEnabled(isOk);

  mName = result;
}

void AddPropertyDialog::OnTypeChanged(const QString& type)
{
  UpdateType();
}

}  // namespace tactile
