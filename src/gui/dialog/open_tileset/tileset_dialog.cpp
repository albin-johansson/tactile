#include "tileset_dialog.hpp"

#include <QFileInfo>

#include "open_tileset_image.hpp"
#include "tactile_error.hpp"
#include "tactile_qstring.hpp"
#include "ui_tileset_dialog.h"

namespace tactile::gui {

tileset_dialog::tileset_dialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::tileset_dialog{}}
{
  m_ui->setupUi(this);
  m_validator = new QIntValidator{1, 1'000, this};

  m_defaultImageIcon = m_ui->imageLabel->pixmap(Qt::ReturnByValueConstant{});
  if (m_defaultImageIcon.isNull()) {
    throw tactile_error{"Could not create default pixmap!"};
  }

  ok_button()->setEnabled(false);
}

tileset_dialog::~tileset_dialog() noexcept
{
  delete m_ui;
}

void tileset_dialog::on_imageButton_pressed()
{
  if (const auto path = open_tileset_image(this); path) {
    const QFileInfo file{*path};
    const auto absoluteFile = file.absoluteFilePath();

    m_image.load(file.absoluteFilePath());

    if (m_image.isNull()) {
      m_ui->imageLabel->setPixmap(m_defaultImageIcon);
      m_ui->sourceEdit->setText(TACTILE_QSTRING(u"Failed to open image!"));
    } else {
      m_ui->imageLabel->setPixmap(QPixmap{absoluteFile}.scaledToHeight(100));
      m_ui->sourceEdit->setText(absoluteFile);
      m_imageName = file.baseName();
      m_path = absoluteFile;
    }

    ok_button()->setEnabled(is_valid());
  }
}

void tileset_dialog::on_widthEdit_textChanged()
{
  validate_input();
}

void tileset_dialog::on_heightEdit_textChanged()
{
  validate_input();
}

auto tileset_dialog::is_valid() const -> bool
{
  return validate(*m_ui->widthEdit) == QValidator::Acceptable &&
         validate(*m_ui->heightEdit) == QValidator::Acceptable &&
         !m_image.isNull();
}

void tileset_dialog::validate_input()
{
  ok_button()->setEnabled(is_valid());
  if (ok_button()->isEnabled()) {
    bool ok{};

    if (const auto width = m_ui->widthEdit->displayText().toInt(&ok); ok) {
      m_tileWidth = tile_width{width};
    }

    if (const auto height = m_ui->heightEdit->displayText().toInt(&ok); ok) {
      m_tileHeight = tile_height{height};
    }
  }
}

auto tileset_dialog::ok_button() -> QPushButton*
{
  return m_ui->buttonBox->button(QDialogButtonBox::Ok);
}

auto tileset_dialog::validate(const QLineEdit& edit) const -> QValidator::State
{
  auto unused = 0;
  auto txt = edit.displayText();
  return m_validator->validate(txt, unused);
}

}  // namespace tactile::gui
