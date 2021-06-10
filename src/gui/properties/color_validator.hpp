#pragma once

#include <QIntValidator>  // QIntValidator

namespace tactile {

class ColorValidator final : public QIntValidator
{
 public:
  explicit ColorValidator(QObject* parent = nullptr);

  [[nodiscard]] auto validate(QString& string, int&) const -> State override;
};

}  // namespace tactile
