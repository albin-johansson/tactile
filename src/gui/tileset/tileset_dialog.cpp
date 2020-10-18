#include "tileset_dialog.hpp"

#include <QFileInfo>

#include "open_tileset_image.hpp"
#include "ui_tileset_dialog.h"

namespace tactile::gui {
namespace {

[[nodiscard]] auto load_pixmap(const QString& path) -> QPixmap
{
  QPixmap pixmap{path};
  pixmap = pixmap.scaledToHeight(100);
  return pixmap;
}

}  // namespace

tileset_dialog::tileset_dialog(QWidget* parent)
    : QDialog{parent},
      m_ui{new Ui::tileset_dialog{}}
{
  m_ui->setupUi(this);
  m_validator = new QIntValidator{1, 1'000, this};

  const auto pixmap = m_ui->imageLabel->pixmap(Qt::ReturnByValueConstant{});
  if (!pixmap.isNull()) {
    m_defaultImageIcon = pixmap;
  } else {
    qWarning("Found no default pixmap for tileset image!");
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
    const auto pathStr = path->path().remove(0, 1);  // remove leading '/'
    const auto fileName = path->fileName();

    m_image.load(pathStr);

    if (m_image.isNull()) {
      m_ui->imageLabel->setPixmap(m_defaultImageIcon);
      m_ui->sourceEdit->setText(QStringLiteral(u"Failed to open image!"));
    } else {
      const QFileInfo info{path->fileName()};
      m_ui->imageLabel->setPixmap(load_pixmap(pathStr));
      m_ui->sourceEdit->setText(info.absoluteFilePath());
      m_imageName = info.baseName();
      m_path = pathStr;
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
      m_width = tile_width{width};
    }

    if (const auto height = m_ui->heightEdit->displayText().toInt(&ok); ok) {
      m_height = tile_height{height};
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
