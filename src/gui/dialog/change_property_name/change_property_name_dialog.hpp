#pragma once

#include <QDialog>
#include <QStandardItemModel>
#include <QValidator>

#include "maybe.hpp"
#include "smart_pointers.hpp"

namespace Ui {
class change_property_name_dialog;
}

namespace tactile::gui {

class change_property_name_dialog final : public QDialog
{
 public:
  explicit change_property_name_dialog(QStandardItemModel* model,
                                       QWidget* parent = nullptr);

  ~change_property_name_dialog() noexcept override;

  [[nodiscard]] static auto spawn(QStandardItemModel* model,
                                  QWidget* parent = nullptr) -> maybe<QString>;

 private:
  unique<Ui::change_property_name_dialog> m_ui;
  QValidator* m_validator{};

 private slots:
  void when_name_changed(const QString& name);
};

}  // namespace tactile::gui
