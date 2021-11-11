#ifndef TACTILE_IO_IR_HELPERS_HPP_
#define TACTILE_IO_IR_HELPERS_HPP_

#include <cstring>  // strcmp

#include <tactile_stdlib.hpp>

#include "ir_common.hpp"

namespace Tactile::IO {

/**
 * \brief Attempts to find a property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property, can safely be null.
 *
 * \return a pointer to the found property; a null pointer is returned otherwise.
 *
 * \see `GetPropertyWithName()`
 * \see `HasPropertyWithName()`
 */
template <typename T>
[[nodiscard]] auto FindPropertyWithName(const T& source, const CStr name)
    -> const Property*
{
  if (!name) {
    return nullptr;
  }

  const auto count = GetPropertyCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& property = GetProperty(source, index);
    const auto* propertyName = GetName(property);
    if (std::strcmp(propertyName, name) == 0) {
      return &property;
    }
  }

  return nullptr;
}

/**
 * \brief Returns the property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property, should not be null.
 *
 * \return a reference to the found property.
 *
 * \throws TactileError if there is no property with the specified name.
 *
 * \see `FindPropertyWithName()`
 * \see `HasPropertyWithName()`
 */
template <typename T>
[[nodiscard]] auto GetPropertyWithName(const T& source, const CStr name)
    -> const Property&
{
  if (const auto* property = FindPropertyWithName(source, name)) {
    return *property;
  }
  else {
    throw TactileError{"Failed to find a property with the specified name!"};
  }
}

/**
 * \brief Indicates whether or not an object has a property with the specified name.
 *
 * \note Avoid the pattern of checking if a property exists with this function followed by
 * a call to `GetPropertyWithName()`, since that will be perform two lookups, i.e. double
 * the work. Use `FindPropertyWithName()` and perform a null check on the returned pointer
 * instead.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property, can safely be null.
 *
 * \return `true` if a property was found; `false` otherwise.
 *
 * \see `FindPropertyWithName()`
 * \see `GetPropertyWithName()`
 */
template <typename T>
[[nodiscard]] auto HasPropertyWithName(const T& source, const CStr name) -> bool
{
  return FindPropertyWithName(source, name) != nullptr;
}

/**
 * \brief Returns the value of the string property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property.
 *
 * \return the string value of the property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
template <typename T>
[[nodiscard]] auto GetString(const T& source, const CStr name) -> CStr
{
  const auto& property = GetPropertyWithName(source, name);
  return GetString(property);
}

/**
 * \brief Returns the value of the integer property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property.
 *
 * \return the integer value of the property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
template <typename T>
[[nodiscard]] auto GetInt(const T& source, const CStr name) -> int32
{
  const auto& property = GetPropertyWithName(source, name);
  return GetInt(property);
}

/**
 * \brief Returns the value of the float property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property.
 *
 * \return the float value of the property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
template <typename T>
[[nodiscard]] auto GetFloat(const T& source, const CStr name) -> float
{
  const auto& property = GetPropertyWithName(source, name);
  return GetFloat(property);
}

/**
 * \brief Returns the value of the boolean property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property.
 *
 * \return the boolean value of the property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
template <typename T>
[[nodiscard]] auto GetBool(const T& source, const CStr name) -> bool
{
  const auto& property = GetPropertyWithName(source, name);
  return GetBool(property);
}

/**
 * \brief Returns the value of the object reference property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property.
 *
 * \return the object ID value of the property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
template <typename T>
[[nodiscard]] auto GetObject(const T& source, const CStr name) -> int32
{
  const auto& property = GetPropertyWithName(source, name);
  return GetObject(property);
}

/**
 * \brief Returns the value of the color property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property.
 *
 * \return the color value of the property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
template <typename T>
[[nodiscard]] auto GetColor(const T& source, const CStr name) -> Color
{
  const auto& property = GetPropertyWithName(source, name);
  return GetColor(property);
}

/**
 * \brief Returns the value of the file path property with the specified name.
 *
 * \tparam T the type of the source object.
 *
 * \param source the source object that contains the relevant properties.
 * \param name the name of the desired property.
 *
 * \return the file path value of the property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
template <typename T>
[[nodiscard]] auto GetFile(const T& source, const CStr name) -> CPathStr
{
  const auto& property = GetPropertyWithName(source, name);
  return GetFile(property);
}

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_HELPERS_HPP_
