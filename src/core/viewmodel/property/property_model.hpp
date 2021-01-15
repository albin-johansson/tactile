#pragma once

#include <QStandardItemModel>

#include "property_manager.hpp"

namespace tactile::vm {

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

  auto add(const QString& name, const core::property& property) -> QModelIndex;

  void change_type(const QString& name, core::property::type type);

  void rename(const QString& oldName, const QString& newName);

  void remove(const QString& name);

  void set_predefined_name(const QString& name);

  [[nodiscard]] auto contains_property(const QString& name) const -> bool;

  [[nodiscard]] auto is_custom_property(const QModelIndex& index) const -> bool;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const core::property&;

 signals:
  void added_color(const QModelIndex& valueIndex);
  void added_file(const QModelIndex& valueIndex);
  void changed_type(const QModelIndex& valueIndex, core::property::type type);

 public slots:
  // Meant to be called as a result of undo/redo
  void added_property(const QString& name);

  void about_to_remove_property(const QString& name);

  void updated_property(const QString& name);

  void renamed_property(const QString& oldName, const QString& newName);

 private:
  core::property_manager* m_manager{};
  QStandardItem* m_predefinedRoot{};
  QStandardItem* m_customRoot{};
  bool m_blockDataChangedSlot{};

  auto add_existing_property_to_gui(const QString& name) -> QModelIndex;

  void remove_property_from_gui(const QString& name);

  auto add_property_to_gui(const QString& name,
                           const core::property& property,
                           QStandardItem* root) -> QModelIndex;

  void rename_property_in_gui(const QString& oldName, const QString& newName);

  void set_value(const QString& name, QStandardItem* item);

 private slots:
  void when_data_changed(const QModelIndex& topLeft,
                         const QModelIndex& bottomRight,
                         const QVector<int>& roles);
};

}  // namespace tactile::vm
