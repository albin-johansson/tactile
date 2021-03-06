#pragma once

#include <QStandardItemModel>  // QStandardItemModel

#include "not_null.hpp"
#include "property_manager.hpp"

namespace tactile::vm {

/**
 * \class property_model
 *
 * \brief Represents the viewmodel responsible for managing properties.
 *
 * \details This class handles a `property_manager` instance and serves as the
 * layer between the GUI and the core model for handling properties.
 *
 * \since 0.2.0
 *
 * \see property_manager
 *
 * \headerfile property_model.hpp
 */
class property_model final : public QStandardItemModel
{
  Q_OBJECT

 public:
  /**
   * \brief Creates a property model based on a property manager.
   *
   * \param manager the associated property manager.
   * \param parent the parent object.
   *
   * \throws tactile_error if `manager` is null.
   *
   * \since 0.2.0
   */
  explicit property_model(not_null<core::property_manager*> manager,
                          QObject* parent = nullptr);

  /**
   * \brief Adds a property to the model.
   *
   * \pre `name` must be unique for the supplied property.
   *
   * \param name the unique name that will be associated with the property.
   * \param type the type of the property that will be added.
   *
   * \since 0.2.0
   */
  void add(const QString& name, core::property::type type);

  /**
   * \brief Adds a property to the model.
   *
   * \pre `name` must be unique for the supplied property.
   *
   * \param name the unique name that will be associated with the property.
   * \param property the property that will be added.
   *
   * \return the index associated with the added property.
   *
   * \since 0.2.0
   */
  auto add(const QString& name, const core::property& property) -> QModelIndex;

  /**
   * \brief Changes the type of an existing property.
   *
   * \pre `name` must be associated with an existing property.
   *
   * \param name the name of the property that will be changed.
   * \param type the new type of the property.
   *
   * \signal `changed_type`
   *
   * \since 0.2.0
   */
  void change_type(const QString& name, core::property::type type);

  /**
   * \brief Renames an existing property.
   *
   * \pre `oldName` must be associated with an existing property.
   * \pre `newName` cannot be associated with an existing property.
   *
   * \post `oldName` can be used for other properties.
   *
   * \param oldName the name of the property that will be renamed.
   * \param newName the new unique name of the property.
   *
   * \since 0.2.0
   */
  void rename(const QString& oldName, const QString& newName);

  /**
   * \brief Removes a property from the model.
   *
   * \pre `name` must be associated with an existing property.
   *
   * \post `name` will be available for use for other properties.
   *
   * \param name the name of the property that will be removed.
   *
   * \since 0.2.0
   */
  void remove(const QString& name);

  void set_root_name(const QString& name);

  /**
   * \brief Indicates whether or not the model contains a property.
   *
   * \param name the name of the property to look for.
   *
   * \return `true` if the model features a property associated with the
   * specified name; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto contains_property(const QString& name) const -> bool;

  /**
   * \brief Indicates whether or not the specified property is a custom
   * property.
   *
   * \param index the index of the property that will be checked.
   *
   * \return `true` if the property is custom; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_custom_property(const QModelIndex& index) const -> bool;

  /**
   * \brief Returns the property associated with the specified name.
   *
   * \pre `name` must be associated with an existing property.
   *
   * \param name the name of the desired property.
   *
   * \return the property associated with the specified name.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto get_property(const QString& name) const
      -> const core::property&;

 signals:
  void added_color(const QModelIndex& valueIndex);
  void added_file(const QModelIndex& valueIndex);
  void changed_type(const QModelIndex& valueIndex, core::property::type type);
  void updated_file(const QModelIndex& valueIndex);
  void updated_color(const QModelIndex& index);

 public slots:
  // Meant to be called as a result of undo/redo
  void added_property(const QString& name);

  void about_to_remove_property(const QString& name);

  void updated_property(const QString& name);

  void changed_property_type(const QString& name);

  void renamed_property(const QString& oldName, const QString& newName);

 private:
  core::property_manager* m_manager{};
  QStandardItem* m_customRoot{};
  bool m_blockDataChanged{};

  void remove_property_from_gui(const QString& name);

  auto add_existing_property_to_gui(const QString& name) -> QModelIndex;

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
