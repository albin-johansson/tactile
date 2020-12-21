#pragma once

#include <QDialog>
#include <QTreeView>
#include <QValidator>
#include <concepts>  // invocable

#include "maybe.hpp"
#include "property.hpp"

namespace Ui {
class add_property_dialog;
}

namespace tactile::gui {

class add_property_dialog final : public QDialog
{
  Q_OBJECT

 public:
  explicit add_property_dialog(QTreeView* tree, QWidget* parent = nullptr);

  ~add_property_dialog() noexcept override;

  template <std::invocable<const QString&, core::property::type> T>
  static void spawn(T&& callable,
                    QTreeView* tree,
                    QWidget* parent = nullptr)
  {
    add_property_dialog dialog{tree, parent};
    if (dialog.exec()) {
      Q_ASSERT(dialog.m_name);
      Q_ASSERT(dialog.m_type);
      callable(*dialog.m_name, *dialog.m_type);
    }
  }

 private:
  Ui::add_property_dialog* m_ui{};
  QValidator* m_nameValidator{};
  maybe<QString> m_name;
  maybe<core::property::type> m_type;

  void update_type();

 private slots:
  void when_type_changed(const QString& type);
  void when_property_name_changed(const QString& name);
};

}  // namespace tactile::gui
