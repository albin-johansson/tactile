#pragma once

#include <QStandardItemModel>  // QStandardItemModel
#include <QValidator>          // QValidator

namespace tactile {

class PropertyNameValidator final : public QValidator
{
 public:
  explicit PropertyNameValidator(QStandardItemModel* model,
                                 QObject* parent = nullptr);

  [[nodiscard]] auto validate(QString& input, int&) const -> State override;

 private:
  QStandardItemModel* mModel{};
};

}  // namespace tactile
