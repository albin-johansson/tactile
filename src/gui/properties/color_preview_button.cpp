#include "color_preview_button.hpp"

#include <QColorDialog>

#include "color_utils.hpp"

namespace tactile {
namespace {

inline const auto fmt_style =
    QStringLiteral(u"background-color: %1; color: %2;");

inline const auto hex_black = QStringLiteral(u"#000000");
inline const auto hex_white = QStringLiteral(u"#FFFFFF");

}  // namespace

ColorPreviewButton::ColorPreviewButton(const QColor color, QWidget* parent)
    : QPushButton{parent}
    , mColor{color}
{
  setObjectName(QStringLiteral(u"ColorPreviewButton"));
  UpdateColor(mColor);

  connect(this, &QPushButton::clicked, this, &ColorPreviewButton::OnClicked);
}

void ColorPreviewButton::UpdateColor(QPushButton& button, const QColor& color)
{
  button.setStyleSheet(fmt_style.arg(color.name(QColor::HexArgb),
                                     IsBright(color) ? hex_black : hex_white));

  // The RGB name is easier to read
  button.setText(color.name(QColor::HexRgb).toUpper());
}

void ColorPreviewButton::SetColor(const QColor& color)
{
  SetRed(color.red());
  SetGreen(color.green());
  SetBlue(color.blue());
  SetAlpha(color.alpha());
}

void ColorPreviewButton::SetRed(const int red)
{
  Q_ASSERT(red >= 0 && red <= 255);
  mColor.setRed(red);
  UpdateColor(mColor);
}

void ColorPreviewButton::SetGreen(const int green)
{
  Q_ASSERT(green >= 0 && green <= 255);
  mColor.setGreen(green);
  UpdateColor(mColor);
}

void ColorPreviewButton::SetBlue(const int blue)
{
  Q_ASSERT(blue >= 0 && blue <= 255);
  mColor.setBlue(blue);
  UpdateColor(mColor);
}

void ColorPreviewButton::SetAlpha(const int alpha)
{
  Q_ASSERT(alpha >= 0 && alpha <= 255);
  mColor.setAlpha(alpha);
  UpdateColor(mColor);
}

auto ColorPreviewButton::CurrentColor() const -> const QColor&
{
  return mColor;
}

void ColorPreviewButton::UpdateColor(const QColor& color)
{
  mColor = color;
  UpdateColor(*this, mColor);
  emit S_ColorChanged(mColor);
}

void ColorPreviewButton::OnClicked()
{
  QColorDialog dialog{mColor, window()};
  dialog.adjustSize();  // Silences setGeometry warnings
  dialog.setOption(QColorDialog::ShowAlphaChannel);
  if (dialog.exec())
  {
    UpdateColor(dialog.selectedColor());
  }
}

}  // namespace tactile
