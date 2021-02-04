#pragma once

#include <QIntValidator>  // QIntValidator

namespace tactile::gui {

class color_validator final : public QIntValidator
{
 public:
  explicit color_validator(QObject* parent = nullptr);

  [[nodiscard]] auto validate(QString& string, int&) const -> State override;
};

}  // namespace tactile::gui
