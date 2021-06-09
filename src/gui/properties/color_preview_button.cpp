#include "color_preview_button.hpp"

#include <QColorDialog>

#include "color_utils.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

color_preview_button::color_preview_button(const QColor color, QWidget* parent)
    : QPushButton{parent}
    , m_color{color}
{
  setObjectName(TACTILE_QSTRING(u"color_preview_dialog"));
  update_color(m_color);

  connect(this, &QPushButton::clicked, [this] {
    QColorDialog dialog{m_color, window()};
    dialog.adjustSize();  // Silences setGeometry warnings
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    if (dialog.exec())
    {
      update_color(dialog.selectedColor());
    }
  });
}

void color_preview_button::update_color(QPushButton& button,
                                        const QColor& color)
{
  static const auto black = TACTILE_QSTRING(u"#000000");
  static const auto white = TACTILE_QSTRING(u"#FFFFFF");

  static const auto fmt = TACTILE_QSTRING(u"background-color: %1; color: %2;");
  button.setStyleSheet(
      fmt.arg(color.name(QColor::HexArgb), is_bright(color) ? black : white));

  // The RGB name is easier to read
  button.setText(color.name(QColor::HexRgb).toUpper());
}

void color_preview_button::set_color(const QColor& color)
{
  set_red(color.red());
  set_green(color.green());
  set_blue(color.blue());
  set_alpha(color.alpha());
}

void color_preview_button::set_red(const int red)
{
  Q_ASSERT(red >= 0 && red <= 255);
  m_color.setRed(red);
  update_color(m_color);
}

void color_preview_button::set_green(const int green)
{
  Q_ASSERT(green >= 0 && green <= 255);
  m_color.setGreen(green);
  update_color(m_color);
}

void color_preview_button::set_blue(const int blue)
{
  Q_ASSERT(blue >= 0 && blue <= 255);
  m_color.setBlue(blue);
  update_color(m_color);
}

void color_preview_button::set_alpha(const int alpha)
{
  Q_ASSERT(alpha >= 0 && alpha <= 255);
  m_color.setAlpha(alpha);
  update_color(m_color);
}

auto color_preview_button::current_color() const -> const QColor&
{
  return m_color;
}

void color_preview_button::update_color(const QColor& color)
{
  m_color = color;
  update_color(*this, m_color);
  emit color_changed(m_color);
}

}  // namespace tactile::gui
