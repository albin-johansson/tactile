#include "resize_dialog.h"

#include <QIntValidator>
#include <QPushButton>
#include <string>

#include "ui_resize_dialog.h"

namespace tactile {

ResizeDialog::ResizeDialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::ResizeDialogUI{}},
      m_chosenWidth{nothing},
      m_chosenHeight{nothing}
{
  m_ui->setupUi(this);

  m_validator = new QIntValidator{1, 20'000, this};
  m_ui->widthEdit->setValidator(m_validator);
  m_ui->heightEdit->setValidator(m_validator);

  connect_line_edit(m_ui->widthEdit);
  connect_line_edit(m_ui->heightEdit);

  ok_button()->setEnabled(false);
}

ResizeDialog::~ResizeDialog() noexcept
{
  delete m_validator;
  delete m_ui;
}

void ResizeDialog::connect_line_edit(QLineEdit* edit) noexcept
{
  if (edit) {
    connect(edit, &QLineEdit::textChanged, this, &ResizeDialog::validate_input);
  }
}

void ResizeDialog::validate_input() noexcept
{
  const auto widthState = is_valid(*m_ui->widthEdit);
  const auto heightState = is_valid(*m_ui->heightEdit);
  ok_button()->setEnabled(widthState == QValidator::Acceptable &&
                          heightState == QValidator::Acceptable);
  if (ok_button()->isEnabled()) {
    m_chosenWidth = std::stoi(m_ui->widthEdit->displayText().toStdString());
    m_chosenHeight = std::stoi(m_ui->heightEdit->displayText().toStdString());
  }
}

QPushButton* ResizeDialog::ok_button() const noexcept
{
  return m_ui->buttonBox->button(QDialogButtonBox::Ok);
}

QValidator::State ResizeDialog::is_valid(const QLineEdit& edit) const noexcept
{
  auto unused = 0;
  auto txt = edit.displayText();
  return m_validator->validate(txt, unused);
}

}  // namespace tactile
