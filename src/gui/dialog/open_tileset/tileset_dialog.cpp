#include "tileset_dialog.hpp"

#include <QFileInfo>

#include "init_ui.hpp"
#include "open_tileset_image.hpp"
#include "tactile_error.hpp"
#include "ui_tileset_dialog.h"

namespace tactile {

TilesetDialog::TilesetDialog(QWidget* parent)
    : QDialog{parent}
    , mUi{InitUi<Ui::TilesetDialog>(this)}
    , mValidator{new QIntValidator{1, 1'000, this}}
{
  mDefaultImageIcon = mUi->imageLabel->pixmap(Qt::ReturnByValueConstant{});
  if (mDefaultImageIcon.isNull())
  {
    throw tactile_error{"Could not create default pixmap!"};
  }

  OkButton()->setEnabled(false);
}

TilesetDialog::~TilesetDialog() noexcept = default;

void TilesetDialog::on_imageButton_pressed()
{
  if (const auto path = OpenTilesetImage(this); path)
  {
    const QFileInfo file{*path};
    const auto absoluteFile = file.absoluteFilePath();

    mImage.load(file.absoluteFilePath());

    if (mImage.isNull())
    {
      mUi->imageLabel->setPixmap(mDefaultImageIcon);
      mUi->sourceEdit->setText(tr("Failed to open image!"));
    }
    else
    {
      mUi->imageLabel->setPixmap(QPixmap{absoluteFile}.scaledToHeight(100));
      mUi->sourceEdit->setText(absoluteFile);
      mImageName = file.baseName();
      mPath = absoluteFile;
    }

    OkButton()->setEnabled(IsValid());
  }
}

void TilesetDialog::on_widthEdit_textChanged()
{
  ValidateInput();
}

void TilesetDialog::on_heightEdit_textChanged()
{
  ValidateInput();
}

auto TilesetDialog::IsValid() const -> bool
{
  return Validate(*mUi->widthEdit) == QValidator::Acceptable &&
         Validate(*mUi->heightEdit) == QValidator::Acceptable &&
         !mImage.isNull();
}

void TilesetDialog::ValidateInput()
{
  OkButton()->setEnabled(IsValid());
  if (OkButton()->isEnabled())
  {
    bool ok{};

    if (const auto width = mUi->widthEdit->displayText().toInt(&ok); ok)
    {
      mTileWidth = tile_width{width};
    }

    if (const auto height = mUi->heightEdit->displayText().toInt(&ok); ok)
    {
      mTileHeight = tile_height{height};
    }
  }
}

auto TilesetDialog::OkButton() -> QPushButton*
{
  return mUi->buttonBox->button(QDialogButtonBox::Ok);
}

auto TilesetDialog::Validate(const QLineEdit& edit) const -> QValidator::State
{
  auto unused = 0;
  auto txt = edit.displayText();
  return mValidator->validate(txt, unused);
}

}  // namespace tactile
