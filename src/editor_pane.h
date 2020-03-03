#pragma once

#include <QWidget>
#include <QRect>

namespace tactile {

class EditorPane final : public QWidget {
 public:
  explicit EditorPane(QWidget* parent = nullptr) noexcept;

 public slots:
  void paint(QPaintEvent* event) noexcept;

 public:
  void paintEvent(QPaintEvent* event) override;

//  void resizeEvent(QResizeEvent* event) override;
};

}  // namespace tactile