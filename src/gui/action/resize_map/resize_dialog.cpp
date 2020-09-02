#include "resize_dialog.hpp"

#include <qpushbutton.h>
#include <qvalidator.h>

#include <string>

#include "ui_resize_dialog.h"

namespace tactile::gui {

resize_dialog::resize_dialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::ResizeDialogUI{}},
      m_chosenWidth{std::nullopt},
      m_chosenHeight{std::nullopt}
{
  m_ui->setupUi(this);

  m_validator = new QIntValidator{1, 20'000, this};
  m_ui->widthEdit->setValidator(m_validator);
  m_ui->heightEdit->setValidator(m_validator);

  connect_line_edit(m_ui->widthEdit);
  connect_line_edit(m_ui->heightEdit);

  ok_button()->setEnabled(false);
}

resize_dialog::~resize_dialog() noexcept
{
  delete m_ui;
}

void resize_dialog::connect_line_edit(QLineEdit* edit)
{
  if (edit) {
    connect(
        edit, &QLineEdit::textChanged, this, &resize_dialog::validate_input);
  }
}

void resize_dialog::validate_input()
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

auto resize_dialog::ok_button() const -> QPushButton*
{
  return m_ui->buttonBox->button(QDialogButtonBox::Ok);
}

auto resize_dialog::is_valid(const QLineEdit& edit) const -> QValidator::State
{
  auto unused = 0;
  auto txt = edit.displayText();
  return m_validator->validate(txt, unused);
}

}  // namespace tactile::gui
