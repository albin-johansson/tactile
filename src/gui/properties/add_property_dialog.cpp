#include "add_property_dialog.hpp"

#include <QPushButton>

#include "property_name_validator.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "ui_add_property_dialog.h"

namespace tactile::gui {

add_property_dialog::add_property_dialog(QStandardItemModel* model,
                                         QWidget* parent)
    : QDialog{parent}
    , m_ui{new Ui::add_property_dialog{}}
    , m_nameValidator{new property_name_validator{model, this}}
{
  m_ui->setupUi(this);

  setObjectName(TACTILE_QSTRING(u"add_property_dialog"));

  // clang-format off
  connect(m_ui->nameEdit, &QLineEdit::textChanged, this, &add_property_dialog::when_name_changed);
  connect(m_ui->typeComboBox, &QComboBox::currentTextChanged, this, &add_property_dialog::when_type_changed);
  // clang-format on

  update_type();
  m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

add_property_dialog::~add_property_dialog() noexcept
{
  delete m_ui;
}

void add_property_dialog::update_type()
{
  const auto text = m_ui->typeComboBox->currentText();
  if (text == TACTILE_QSTRING(u"string")) {
    m_type = core::property::string;

  } else if (text == TACTILE_QSTRING(u"int")) {
    m_type = core::property::integer;

  } else if (text == TACTILE_QSTRING(u"float")) {
    m_type = core::property::floating;

  } else if (text == TACTILE_QSTRING(u"bool")) {
    m_type = core::property::boolean;

  } else if (text == TACTILE_QSTRING(u"color")) {
    m_type = core::property::color;

  } else if (text == TACTILE_QSTRING(u"object")) {
    m_type = core::property::object;

  } else if (text == TACTILE_QSTRING(u"file")) {
    m_type = core::property::file;

  } else {
    throw tactile_error{"Did not recognize property type!"};
  }
}

void add_property_dialog::when_name_changed(const QString& name)
{
  auto result = name;
  auto unused = 0;
  const auto isOk =
      m_nameValidator->validate(result, unused) == QValidator::Acceptable;

  auto* okButton = m_ui->buttonBox->button(QDialogButtonBox::Ok);
  okButton->setEnabled(isOk);

  m_name = result;
}

void add_property_dialog::when_type_changed(const QString&)
{
  update_type();
}

}  // namespace tactile::gui
