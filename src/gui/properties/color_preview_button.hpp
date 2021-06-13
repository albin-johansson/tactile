#pragma once

#include <QColor>       // QColor
#include <QPushButton>  // QPushButton

namespace tactile {

class ColorPreviewButton final : public QPushButton
{
  Q_OBJECT

 public:
  explicit ColorPreviewButton(QColor color, QWidget* parent = nullptr);

  static void UpdateColor(QPushButton& button, const QColor& color);

  void SetColor(const QColor& color);

  void SetRed(int red);

  void SetGreen(int green);

  void SetBlue(int blue);

  void SetAlpha(int alpha);

  [[nodiscard]] auto CurrentColor() const -> const QColor&;

 signals:
  void S_ColorChanged(const QColor& color);

 private:
  QColor mColor;

  void UpdateColor(const QColor& color);

 private slots:
  void OnClicked();
};

}  // namespace tactile
