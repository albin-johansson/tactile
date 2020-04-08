#include "tile_sheet_dialog.h"

#include <QLineEdit>

#include "open_tile_sheet_image.h"
#include "ui_tile_sheet_dialog.h"

namespace tactile {

TileSheetDialog::TileSheetDialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::TileSheetDialogUI{}},
      m_image{std::make_shared<QImage>()},
      m_width{nothing},
      m_height{nothing}
{
  m_ui->setupUi(this);
  m_validator = std::make_unique<QIntValidator>(1, 1'000, this);
  if (m_ui->imageLabel->pixmap()) {
    m_defaultImageIcon = *m_ui->imageLabel->pixmap();
  } else {
    qWarning("Found no default pixmap for tile sheet image!");
  }

  connect(m_ui->widthEdit,
          &QLineEdit::textChanged,
          this,
          &TileSheetDialog::validate_input);

  connect(m_ui->heightEdit,
          &QLineEdit::textChanged,
          this,
          &TileSheetDialog::validate_input);

  connect(m_ui->imageButton, &QPushButton::pressed, this, [this] {
    const auto path = open_tile_sheet_image(this);
    if (path) {
      m_image->load(*path);

      if (m_image->isNull()) {
        // TODO indicate that something went wrong
        m_ui->imageLabel->setPixmap(m_defaultImageIcon);
      } else  {
        m_ui->imageLabel->setPixmap(load_pixmap(*path));
      }

      ok_button()->setEnabled(is_valid());
    }
  });
}

TileSheetDialog::~TileSheetDialog() noexcept
{
  delete m_ui;
}

bool TileSheetDialog::is_valid() const noexcept
{
  return validate(*m_ui->widthEdit) == QValidator::Acceptable &&
         validate(*m_ui->heightEdit) == QValidator::Acceptable &&
         !m_image->isNull();
}

QPixmap TileSheetDialog::load_pixmap(const QString& path) const noexcept
{
  QPixmap pixmap{path};
  pixmap = pixmap.scaledToHeight(100);
  return pixmap;
}

void TileSheetDialog::validate_input() noexcept
{
  ok_button()->setEnabled(is_valid());
  if (ok_button()->isEnabled()) {
    m_width = std::stoi(m_ui->widthEdit->displayText().toStdString());
    m_height = std::stoi(m_ui->heightEdit->displayText().toStdString());
  }
}

QPushButton* TileSheetDialog::ok_button() noexcept
{
  return m_ui->buttonBox->button(QDialogButtonBox::Ok);
}

QValidator::State TileSheetDialog::validate(const QLineEdit& edit) const
    noexcept
{
  auto unused = 0;
  auto txt = edit.displayText();
  return m_validator->validate(txt, unused);
}

}  // namespace tactile
