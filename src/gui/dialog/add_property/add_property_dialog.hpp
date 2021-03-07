#pragma once

#include <QDialog>             // QDialog
#include <QStandardItemModel>  // QStandardItemModel
#include <QValidator>          // QValidator
#include <concepts>            // invocable

#include "maybe.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"

namespace Ui {
class add_property_dialog;
}

namespace tactile::gui {

class add_property_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit add_property_dialog(QStandardItemModel* model,
                               QWidget* parent = nullptr);

  ~add_property_dialog() noexcept override;

  template <std::invocable<const QString&, core::property_type> T>
  static void spawn(T&& callable,
                    QStandardItemModel* model,
                    QWidget* parent = nullptr)
  {
    add_property_dialog dialog{model, parent};
    if (dialog.exec()) {
      Q_ASSERT(dialog.m_name);
      Q_ASSERT(dialog.m_type);
      callable(*dialog.m_name, *dialog.m_type);
    }
  }

 private:
  unique<Ui::add_property_dialog> m_ui;
  QValidator* m_nameValidator{};
  maybe<QString> m_name;
  maybe<core::property_type> m_type;

  void update_type();

 private slots:
  void when_name_changed(const QString& name);
  void when_type_changed(const QString& type);
};

}  // namespace tactile::gui
