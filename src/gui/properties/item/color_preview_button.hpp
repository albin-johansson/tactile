#pragma once

#include <QPushButton>

namespace tactile::gui {

class color_preview_button final : public QPushButton
{
  Q_OBJECT

 public:
  explicit color_preview_button(QColor color, QWidget* parent = nullptr);

  void set_color(const QColor& color);

  void set_red(int red);

  void set_green(int green);

  void set_blue(int blue);

  void set_alpha(int alpha);

  [[nodiscard]] auto current_color() const -> const QColor&;

 signals:
  void color_changed(const QColor& color);

 private:
  QColor m_color;

  void update_color(const QColor& color);
};

}  // namespace tactile::gui
