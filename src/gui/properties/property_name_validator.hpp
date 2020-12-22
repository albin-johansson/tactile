#pragma once

#include <QStandardItemModel>
#include <QValidator>

namespace tactile::gui {

class property_name_validator final : public QValidator
{
 public:
  explicit property_name_validator(QStandardItemModel* model,
                                   QObject* parent = nullptr);

  [[nodiscard]] auto validate(QString& input, int&) const -> State override;

 private:
  QStandardItemModel* m_model{};
};

}  // namespace tactile::gui
