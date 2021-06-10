#include "add_property_dialog.hpp"

#include <QPushButton>

#include "init_ui.hpp"
#include "property_name_validator.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "ui_add_property_dialog.h"

namespace tactile {

AddPropertyDialog::AddPropertyDialog(QStandardItemModel* model, QWidget* parent)
    : QDialog{parent}
    , mUi{InitUi<Ui::AddPropertyDialog>(this)}
    , mNameValidator{new PropertyNameValidator{model, this}}
{
  setObjectName(TACTILE_QSTRING(u"add_property_dialog"));

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
  if (text == TACTILE_QSTRING(u"string"))
  {
    mType = core::PropertyType::string;
  }
  else if (text == TACTILE_QSTRING(u"int"))
  {
    mType = core::PropertyType::integer;
  }
  else if (text == TACTILE_QSTRING(u"float"))
  {
    mType = core::PropertyType::floating;
  }
  else if (text == TACTILE_QSTRING(u"bool"))
  {
    mType = core::PropertyType::boolean;
  }
  else if (text == TACTILE_QSTRING(u"color"))
  {
    mType = core::PropertyType::color;
  }
  else if (text == TACTILE_QSTRING(u"object"))
  {
    mType = core::PropertyType::object;
  }
  else if (text == TACTILE_QSTRING(u"file"))
  {
    mType = core::PropertyType::file;
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
