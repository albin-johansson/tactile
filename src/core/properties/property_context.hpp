#pragma once

#include <string>          // string
#include <string_view>     // string_view
#include <vector_map.hpp>  // vector_map

#include "aliases/ints.hpp"
#include "property.hpp"
#include "property_type.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

using PropertyMap = rune::vector_map<std::string, Property>;

/**
 * \interface IPropertyContext
 *
 * \brief An interface for objects that support properties.
 */
class IPropertyContext
{
 public:
  virtual ~IPropertyContext() = default;

  /**
   * \brief Adds a property with the specified name and type.
   *
   * \pre `name` must not be associated with a property.
   *
   * \details The added property will use its default value.
   *
   * \param name the name that will be associated with the property.
   * \param type the type of the property.
   */
  virtual void AddProperty(std::string name, PropertyType type) = 0;

  /**
   * \brief Adds a property with the specified name and value.
   *
   * \pre `name` must not be associated with a property.
   *
   * \param name the name that will be associated with the property.
   * \param property the value of the property.
   */
  virtual void AddProperty(std::string name, const Property& property) = 0;

  /**
   * \brief Removes the property associated with the specified name.
   *
   * \pre `name` must be associated with a property.
   *
   * \param name the name of the property that will be removed.
   */
  virtual void RemoveProperty(std::string_view name) = 0;

  /**
   * \brief Changes the name associated with a property.
   *
   * \pre `oldName` must be associated with a property.
   * \pre `newName` must not be associated with a property.
   *
   * \param oldName the name that used to be associated with the property.
   * \param newName the new name that will be associated with the property.
   */
  virtual void RenameProperty(std::string_view oldName, std::string newName) = 0;

  /**
   * \brief Sets the value of a property.
   *
   * \pre `name` must be associated with a property.
   * \pre `property` must be of the same type as the previous value.
   *
   * \param name the name of the property that will be changed.
   * \param property the new value of the property.
   */
  virtual void SetProperty(std::string_view name, const Property& property) = 0;

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
   */
  virtual void ChangePropertyType(std::string name, PropertyType type) = 0;

  /**
   * \brief Indicates whether or not there is a property associated with the
   * specified name.
   *
   * \param name the name of the property to look for.
   *
   * \return `true` if there is a property associated with the specified name;
   * `false` otherwise.
   */
  [[nodiscard]] virtual auto HasProperty(std::string_view name) const -> bool = 0;

  /**
   * \brief Returns the property associated with the specified name.
   *
   * \note This function throws if there is no property associated with the
   * specified name.
   *
   * \param name the name associated with the desired property.
   *
   * \return the property associated with `name`.
   */
  [[nodiscard]] virtual auto GetProperty(std::string_view name) const
      -> const Property& = 0;

  /**
   * \brief Returns all properties associated with the context.
   *
   * \return all properties associated with the context.
   */
  [[nodiscard]] virtual auto GetProperties() const -> const PropertyMap& = 0;

  /**
   * \brief Returns the current amount of properties.
   *
   * \return the amount of properties.
   */
  [[nodiscard]] virtual auto GetPropertyCount() const -> usize = 0;

  /**
   * \brief Returns a human-readable name associated with the context.
   *
   * \return the name of the context.
   */
  [[nodiscard]] virtual auto GetName() const -> const std::string& = 0;
};

/// \} End of group core

}  // namespace Tactile
