#pragma once

#include <QStandardItemModel>

#include "property_manager.hpp"

namespace tactile::viewmodel {

class property_viewmodel final : public QStandardItemModel
{
  Q_OBJECT

 public:
  explicit property_viewmodel(core::property_manager* manager,
                              QObject* parent = nullptr);

  void clear_predefined();

  void add_predefined(const QString& name, const core::property& property);

  void add(const QString& name, core::property::type type);

  void set_predefined_name(const QString& name);

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

  void add_property(const QString& name,
                    const core::property& property,
                    QStandardItem* root);

  void add_string(const QString& name,
                  const core::property& property,
                  QStandardItem* root);

  void add_int(const QString& name,
               const core::property& property,
               QStandardItem* root);

  void add_float(const QString& name,
                 const core::property& property,
                 QStandardItem* root);

  void add_bool(const QString& name,
                const core::property& property,
                QStandardItem* root);

  void add_object(const QString& name,
                  const core::property& property,
                  QStandardItem* root);

  void add_color(const QString& name,
                 const core::property& property,
                 QStandardItem* root);

  void add_file(const QString& name,
                const core::property& property,
                QStandardItem* root);

 private slots:
  void when_item_changed(QStandardItem* item);
};

}  // namespace tactile::viewmodel
