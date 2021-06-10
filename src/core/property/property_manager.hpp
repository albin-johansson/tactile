#pragma once

#include <QString>  // QString

#include "property.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * \interface IPropertyManager
 *
 * \brief An interface for objects that support GetProperties.
 *
 * \since 0.2.0
 *
 * \headerfile property_manager.hpp
 */
class IPropertyManager
{
 public:
  using property_map = vector_map<QString, property>;

  virtual ~IPropertyManager() noexcept = default;

  /**
   * \brief Adds a property with the specified name and type.
   *
   * \pre `name` must not be associated with a property.
   *
   * \details The added property will use its default value.
   *
   * \param name the name that will be associated with the property.
   * \param type the type of the property.
   *
   * \since 0.2.0
   */
  virtual void AddProperty(const QString& name, property_type type) = 0;

  /**
   * \brief Adds a property with the specified name and value.
   *
   * \pre `name` must not be associated with a property.
   *
   * \param name the name that will be associated with the property.
   * \param type the value of the property.
   *
   * \since 0.2.0
   */
  virtual void AddProperty(const QString& name, const property& property) = 0;

  /**
   * \brief Removes the property associated with the specified name.
   *
   * \pre `name` must be associated with a property.
   *
   * \param name the name of the property that will be removed.
   *
   * \since 0.2.0
   */
  virtual void RemoveProperty(const QString& name) = 0;

  /**
   * \brief Changes the name associated with a property.
   *
   * \pre `oldName` must be associated with a property.
   * \pre `newName` must not be associated with a property.
   *
   * \param oldName the name that used to be associated with the property.
   * \param newName the new name that will be associated with the property.
   *
   * \since 0.2.0
   */
  virtual void RenameProperty(const QString& oldName,
                              const QString& newName) = 0;

  /**
   * \brief Sets the value of a property.
   *
   * \pre `name` must be associated with a property.
   * \pre `property` must be of the same type as the previous value.
   *
   * \param name the name of the property that will be changed.
   * \param property the new value of the property.
   *
   * \since 0.2.0
   */
  virtual void SetProperty(const QString& name,
                           const core::property& property) = 0;

  /**
   * \brief Changes the type of an existing property.
   *
   * \details The changed property will assume the default value for the new
   * type.
   *
   * \pre `name` must be associated with a property.
   * \pre The supplied type must be different to the current type of the
   * property.
   *
   * \param name the name associated with the property that will be changed.
   * \param type the new type of the property.
   *
   * \since 0.2.0
   */
  virtual void ChangePropertyType(const QString& name,
                                  core::property_type type) = 0;

  /**
   * \brief Returns the property associated with the specified name.
   *
   * \note This function throws if there is no property associated with the
   * specified name.
   *
   * \param name the name associated with the desired property.
   *
   * \return the property associated with `name`.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto GetProperty(const QString& name) const
      -> const property& = 0;

  /**
   * \copydoc property_manager::GetProperty
   */
  [[nodiscard]] virtual auto GetProperty(const QString& name) -> property& = 0;

  /**
   * \brief Indicates whether or not there is a property associated with the
   * specified name.
   *
   * \param name the name of the property to look for.
   *
   * \return `true` if there is a property associated with the specified name;
   * `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto HasProperty(const QString& name) const -> bool = 0;

  /**
   * \brief Returns the amount of GetProperties handled by the manager.
   *
   * \return the current amount of GetProperties.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto PropertyCount() const noexcept -> int = 0;

  [[nodiscard]] virtual auto GetProperties() const -> const property_map& = 0;
};

}  // namespace tactile::core
