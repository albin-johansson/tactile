#pragma once

#include <QStandardItemModel>

#include "property_manager.hpp"

namespace tactile::viewmodel {

class property_model final : public QStandardItemModel
{
  Q_OBJECT

 public:
  explicit property_model(core::property_manager* manager,
                          QObject* parent = nullptr);

  void clear_predefined();

  auto add_predefined(const QString& name,
                      const core::property& property,
                      bool readOnly = true) -> QModelIndex;

  auto add(const QString& name, core::property::type type) -> QModelIndex;

  void rename(const QString& oldName, const QString& newName);

  void remove(const QString& name);

  void set_predefined_name(const QString& name);

  [[nodiscard]] auto is_custom_property(const QModelIndex& index) const -> bool;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const core::property&;

 signals:
  void added_string(const QModelIndex& valueIndex);
  void added_int(const QModelIndex& valueIndex);
  void added_float(const QModelIndex& valueIndex);
  void added_bool(const QModelIndex& valueIndex);
  void added_object(const QModelIndex& valueIndex);
  void added_color(const QModelIndex& valueIndex);
  void added_file(const QModelIndex& valueIndex);

 private:
  core::property_manager* m_manager{};
  QStandardItem* m_predefinedRoot{};
  QStandardItem* m_customRoot{};

  auto add_property(const QString& name,
                    const core::property& property,
                    QStandardItem* root) -> QModelIndex;

  auto add_string(const QString& name,
                  const core::property& property,
                  QStandardItem* root) -> QModelIndex;

  auto add_int(const QString& name,
               const core::property& property,
               QStandardItem* root) -> QModelIndex;

  auto add_float(const QString& name,
                 const core::property& property,
                 QStandardItem* root) -> QModelIndex;

  auto add_bool(const QString& name,
                const core::property& property,
                QStandardItem* root) -> QModelIndex;

  auto add_object(const QString& name,
                  const core::property& property,
                  QStandardItem* root) -> QModelIndex;

  auto add_color(const QString& name,
                 const core::property& property,
                 QStandardItem* root) -> QModelIndex;

  auto add_file(const QString& name,
                const core::property& property,
                QStandardItem* root) -> QModelIndex;

  void update_name(const QString& oldName, const QString& newName);

  void set_value(const QString& name, QStandardItem* item);

 private slots:
  void when_item_changed(QStandardItem* item);
};

}  // namespace tactile::viewmodel
