#pragma once

#include <QDialog>             // QDialog
#include <QStandardItemModel>  // QStandardItemModel
#include <QValidator>          // QValidator

#include "forward_declare.hpp"
#include "maybe.hpp"
#include "smart_pointers.hpp"

TACTILE_FORWARD_DECLARE_UI(ChangePropertyNameDialog)

namespace tactile {

class ChangePropertyNameDialog final : public QDialog
{
 public:
  explicit ChangePropertyNameDialog(QStandardItemModel* model,
                                    QWidget* parent = nullptr);

  ~ChangePropertyNameDialog() noexcept override;

  [[nodiscard]] static auto Spawn(QStandardItemModel* model,
                                  QWidget* parent = nullptr) -> Maybe<QString>;

 private:
  unique<Ui::ChangePropertyNameDialog> mUi;
  QValidator* mValidator{};

 private slots:
  void OnNameChanged(const QString& name);
};

}  // namespace tactile
