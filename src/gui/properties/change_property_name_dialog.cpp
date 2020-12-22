#include "change_property_name_dialog.hpp"

#include <QPushButton>

#include "property_name_validator.hpp"
#include "ui_change_property_name_dialog.h"

namespace tactile::gui {

change_property_name_dialog::change_property_name_dialog(
    QStandardItemModel* model,
    QWidget* parent)
    : QDialog{parent}
    , m_ui{new Ui::change_property_name_dialog{}}
    , m_validator{new property_name_validator{model, this}}
{
  m_ui->setupUi(this);

  // clang-format off
  connect(m_ui->nameEdit, &QLineEdit::textChanged, this, &change_property_name_dialog::when_name_changed);
  // clang-format on

  m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

change_property_name_dialog::~change_property_name_dialog() noexcept
{
  delete m_ui;
}

auto change_property_name_dialog::spawn(QStandardItemModel* model,
                                        QWidget* parent) -> maybe<QString>
{
  change_property_name_dialog dialog{model, parent};
  if (dialog.exec()) {
    return dialog.m_ui->nameEdit->text();
  } else {
    return std::nullopt;
  }
}

void change_property_name_dialog::when_name_changed(const QString& name)
{
  auto result = name;
  auto unused = 0;
  const auto ok =
      m_validator->validate(result, unused) == QValidator::Acceptable;
  m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}

}  // namespace tactile::gui
