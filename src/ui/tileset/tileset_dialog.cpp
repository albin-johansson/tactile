#include "tileset_dialog.hpp"

#include <QLineEdit>

#include "open_tileset_image.hpp"
#include "ui_tileset_dialog.h"

namespace tactile::ui {
namespace {

[[nodiscard]] auto load_pixmap(const QString& path) -> QPixmap
{
  QPixmap pixmap{path};
  pixmap = pixmap.scaledToHeight(100);
  return pixmap;
}

}  // namespace

tileset_dialog::tileset_dialog(QWidget* parent)
    : QDialog{parent}, m_ui{new Ui::TilesetDialogUI{}}
{
  m_ui->setupUi(this);
  m_validator = new QIntValidator{1, 1'000, this};

  const auto pixmap = m_ui->imageLabel->pixmap(Qt::ReturnByValueConstant{});
  if (!pixmap.isNull()) {
    m_defaultImageIcon = pixmap;
  } else {
    qWarning("Found no default pixmap for tileset image!");
  }

  connect(m_ui->widthEdit,
          &QLineEdit::textChanged,
          this,
          &tileset_dialog::validate_input);

  connect(m_ui->heightEdit,
          &QLineEdit::textChanged,
          this,
          &tileset_dialog::validate_input);

  connect(m_ui->imageButton,
          &QPushButton::pressed,
          this,
          &tileset_dialog::handle_image_select);
}

tileset_dialog::~tileset_dialog() noexcept
{
  delete m_ui;
}

void tileset_dialog::handle_image_select()
{
  const auto path = open_tileset_image(this);

  if (path) {
    const auto pathStr = path->path().remove(0, 1);  // remove leading '/'
    const auto fileName = path->fileName();

    m_image.load(pathStr);

    if (m_image.isNull()) {
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
}

auto tileset_dialog::chosen_image() const noexcept -> const QImage&
{
  return m_image;
}

auto tileset_dialog::chosen_width() const noexcept -> std::optional<int>
{
  return m_width;
}

auto tileset_dialog::chosen_height() const noexcept -> std::optional<int>
{
  return m_height;
}

auto tileset_dialog::image_name() const noexcept -> std::optional<QString>
{
  return m_imageName;
}

auto tileset_dialog::is_valid() const noexcept -> bool
{
  return validate(*m_ui->widthEdit) == QValidator::Acceptable &&
         validate(*m_ui->heightEdit) == QValidator::Acceptable &&
         !m_image.isNull();
}

void tileset_dialog::validate_input() noexcept
{
  ok_button()->setEnabled(is_valid());
  if (ok_button()->isEnabled()) {
    m_width = std::stoi(m_ui->widthEdit->displayText().toStdString());
    m_height = std::stoi(m_ui->heightEdit->displayText().toStdString());
  }
}

auto tileset_dialog::ok_button() noexcept -> QPushButton*
{
  return m_ui->buttonBox->button(QDialogButtonBox::Ok);
}

auto tileset_dialog::validate(const QLineEdit& edit) const noexcept
    -> QValidator::State
{
  auto unused = 0;
  auto txt = edit.displayText();
  return m_validator->validate(txt, unused);
}

}  // namespace tactile::ui
