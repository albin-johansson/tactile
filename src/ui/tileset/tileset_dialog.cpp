#include "tileset_dialog.hpp"

#include <QLineEdit>

#include "open_tileset_image.hpp"
#include "ui_tileset_dialog.h"

namespace tactile::ui {

TilesetDialog::TilesetDialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::TilesetDialogUI{}},
      m_image{std::make_shared<QImage>()},
      m_width{std::nullopt},
      m_height{std::nullopt}
{
  m_ui->setupUi(this);
  m_validator = std::make_unique<QIntValidator>(1, 1'000, this);

  const auto pixmap = m_ui->imageLabel->pixmap(Qt::ReturnByValueConstant{});
  if (!pixmap.isNull()) {
    m_defaultImageIcon = pixmap;
  } else {
    qWarning("Found no default pixmap for tileset image!");
  }

  connect(m_ui->widthEdit,
          &QLineEdit::textChanged,
          this,
          &TilesetDialog::validate_input);

  connect(m_ui->heightEdit,
          &QLineEdit::textChanged,
          this,
          &TilesetDialog::validate_input);

  connect(m_ui->imageButton, &QPushButton::pressed, this, [this] {
    const auto path = open_tileset_image(this);
    if (path) {
      const auto pathStr = path->path().remove(0, 1);  // remove leading '/'
      const auto fileName = path->fileName();

      m_image->load(pathStr);

      if (m_image->isNull()) {
        m_ui->imageLabel->setPixmap(m_defaultImageIcon);
        m_ui->imageInfoLabel->setText("Failed to open image: " + fileName);
        m_ui->imageInfoLabel->setStyleSheet("QLabel { color : red; }");
      } else {
        m_ui->imageLabel->setPixmap(load_pixmap(pathStr));
        m_imageName = fileName;
        m_ui->imageInfoLabel->setText(fileName);
        m_ui->imageInfoLabel->setStyleSheet("QLabel { color : black; }");
      }

      ok_button()->setEnabled(is_valid());
    }
  });
}

TilesetDialog::~TilesetDialog() noexcept
{
  delete m_ui;
}

bool TilesetDialog::is_valid() const noexcept
{
  return validate(*m_ui->widthEdit) == QValidator::Acceptable &&
         validate(*m_ui->heightEdit) == QValidator::Acceptable &&
         !m_image->isNull();
}

QPixmap TilesetDialog::load_pixmap(const QString& path) const noexcept
{
  QPixmap pixmap{path};
  pixmap = pixmap.scaledToHeight(100);
  return pixmap;
}

void TilesetDialog::validate_input() noexcept
{
  ok_button()->setEnabled(is_valid());
  if (ok_button()->isEnabled()) {
    m_width = std::stoi(m_ui->widthEdit->displayText().toStdString());
    m_height = std::stoi(m_ui->heightEdit->displayText().toStdString());
  }
}

QPushButton* TilesetDialog::ok_button() noexcept
{
  return m_ui->buttonBox->button(QDialogButtonBox::Ok);
}

QValidator::State TilesetDialog::validate(const QLineEdit& edit) const noexcept
{
  auto unused = 0;
  auto txt = edit.displayText();
  return m_validator->validate(txt, unused);
}

}  // namespace tactile::ui
