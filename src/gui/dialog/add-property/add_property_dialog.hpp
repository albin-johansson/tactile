#pragma once

#include <QDialog>   // QDialog
#include <concepts>  // invocable

#include "maybe.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"
#include "tactile_declare_ui.hpp"

class QStandardItemModel;
class QValidator;

TACTILE_DECLARE_UI(AddPropertyDialog)

namespace tactile {

class AddPropertyDialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit AddPropertyDialog(QStandardItemModel* model,
                             QWidget* parent = nullptr);

  ~AddPropertyDialog() noexcept override;

  template <std::invocable<const QString&, core::PropertyType> T>
  static void Spawn(T&& callable,
                    QStandardItemModel* model,
                    QWidget* parent = nullptr)
  {
    AddPropertyDialog dialog{model, parent};
    if (dialog.exec())
    {
      Q_ASSERT(dialog.mName);
      Q_ASSERT(dialog.mType);
      callable(*dialog.mName, *dialog.mType);
    }
  }

 private:
  Unique<Ui::AddPropertyDialog> mUi;
  QValidator* mNameValidator{};
  Maybe<QString> mName;
  Maybe<core::PropertyType> mType;

  void UpdateType();

 private slots:
  void OnNameChanged(const QString& name);
  void OnTypeChanged(const QString& type);
};

}  // namespace tactile
