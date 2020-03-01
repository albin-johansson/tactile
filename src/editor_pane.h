#pragma once

#include <QWidget>
#include <QRect>

namespace tactile {

class EditorPane final : public QWidget {
 public:
  explicit EditorPane(QWidget* parent = nullptr) noexcept;

};

}  // namespace tactile