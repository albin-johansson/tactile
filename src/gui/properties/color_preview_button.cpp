#include "color_preview_button.hpp"

#include <QColorDialog>
#include <utility>  // move

#include "tactile_qstring.hpp"

namespace tactile::gui {

color_preview_button::color_preview_button(QColor color, QWidget* parent)
    : QPushButton{parent}
    , m_color{std::move(color)}
{
  setObjectName(TACTILE_QSTRING(u"color_preview_dialog"));
  update_color(m_color);

  connect(this, &QPushButton::clicked, [this] {
    QColorDialog dialog{m_color, window()};
    dialog.adjustSize();  // Silences setGeometry warnings
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    if (dialog.exec()) {
      update_color(dialog.selectedColor());
    }
  });
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
  static const auto fmt = TACTILE_QSTRING(u"background-color: %1; color: %2;");
  static const auto black = TACTILE_QSTRING(u"#000000");
  static const auto white = TACTILE_QSTRING(u"#FFFFFF");

  m_color = color;
  const auto name = m_color.name(QColor::HexArgb);

  const auto intensity = (m_color.red() * 0.299) + (m_color.green() * 0.587) +
                         (m_color.blue() * 0.114);
  if (intensity > 186) {
    setStyleSheet(fmt.arg(name).arg(black));
  } else {
    setStyleSheet(fmt.arg(name).arg(white));
  }

  setText(name.toUpper());

  emit color_changed(m_color);
}

}  // namespace tactile::gui
