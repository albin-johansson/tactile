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
class PropertyModel final : public QStandardItemModel
{
  Q_OBJECT

 public:
  /**
   * \brief Creates a property model based on a property manager.
   *
   * \param manager the associated property manager.
   * \param parent the parent object.
   *
   * \throws TactileError if `manager` is null.
   *
   * \since 0.2.0
   */
  explicit PropertyModel(not_null<core::IPropertyManager*> manager,
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
  void Add(const QString& name, core::PropertyType type);

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
  auto Add(const QString& name, const core::Property& property) -> QModelIndex;

  /**
   * \brief Changes the type of an existing property.
   *
   * \pre `name` must be associated with an existing property.
   *
   * \param name the name of the property that will be changed.
   * \param type the new type of the property.
   *
   * \signal `S_ChangedType`
   *
   * \since 0.2.0
   */
  void ChangeType(const QString& name, core::PropertyType type);

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
  void Rename(const QString& oldName, const QString& newName);

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
  void Remove(const QString& name);

  void SetRootName(const QString& name);

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
  [[nodiscard]] auto ContainsProperty(const QString& name) const -> bool;

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
  [[nodiscard]] auto IsCustomProperty(const QModelIndex& index) const -> bool;

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
  [[nodiscard]] auto GetProperty(const QString& name) const
      -> const core::Property&;

 signals:
  void S_AddedColor(const QModelIndex& valueIndex);
  void S_AddedFile(const QModelIndex& valueIndex);
  void S_ChangedType(const QModelIndex& valueIndex, core::PropertyType type);
  void S_UpdatedFile(const QModelIndex& valueIndex);
  void S_UpdatedColor(const QModelIndex& index);

 public slots:
  // Meant to be called as a result of undo/redo
  void OnAddedProperty(const QString& name);
  void OnAboutToRemoveProperty(const QString& name);
  void OnUpdatedProperty(const QString& name);
  void OnChangedPropertyType(const QString& name);
  void OnRenamedProperty(const QString& oldName, const QString& newName);

 private:
  core::IPropertyManager* mManager{};
  QStandardItem* mCustomRoot{};
  bool mBlockDataChanged{};

  void RemovePropertyFromGui(const QString& name);

  auto AddExistingPropertyToGui(const QString& name) -> QModelIndex;

  auto AddPropertyToGui(const QString& name,
                        const core::Property& property,
                        QStandardItem* root) -> QModelIndex;

  void RenamePropertyInGui(const QString& oldName, const QString& newName);

  void SetValue(const QString& name, QStandardItem* item);

 private slots:
  void OnDataChanged(const QModelIndex& topLeft,
                     const QModelIndex& bottomRight,
                     const QVector<int>& roles);
};

}  // namespace tactile::vm
