#ifndef TACTILE_IO_IR_PROPERTY_HPP_
#define TACTILE_IO_IR_PROPERTY_HPP_

#include <tactile_def.hpp>

#include "api.hpp"
#include "ir_common.hpp"

namespace tactile::IO {

/// \name Property API
/// \{

/**
 * \brief Sets the name of a property.
 *
 * \pre The supplied string cannot be null.
 *
 * \note This function will use a deep copy of the supplied string.
 *
 * \param property the property that will be modified.
 * \param name the new property name.
 */
TACTILE_IO_API
void SetName(Property& property, CStr name);

/**
 * \brief Sets the value of a property to an integer.
 *
 * \param property the property that will be modified.
 * \param value the new value of the property.
 */
TACTILE_IO_API
void AssignInt(Property& property, int32 value);

/**
 * \brief Sets the value of a property to a float.
 *
 * \param property the property that will be modified.
 * \param value the new value of the property.
 */
TACTILE_IO_API
void AssignFloat(Property& property, float value);

/**
 * \brief Sets the value of a property to a string.
 *
 * \pre The supplied string cannot be null.
 *
 * \note This function will use a deep copy of the supplied string.
 *
 * \param property the property that will be modified.
 * \param value the new value of the property.
 */
TACTILE_IO_API
void AssignString(Property& property, CStr str);

/**
 * \brief Sets the value of a property to a boolean.
 *
 * \param property the property that will be modified.
 * \param value the new value of the property.
 */
TACTILE_IO_API
void AssignBool(Property& property, bool value);

/**
 * \brief Sets the value of a property to a file path.
 *
 * \pre The supplied string cannot be null.
 *
 * \note This function will use a deep copy of the supplied string.
 *
 * \param property the property that will be modified.
 * \param value the new value of the property.
 */
TACTILE_IO_API
void AssignFile(Property& property, CPathStr path);

/**
 * \brief Sets the value of a property to an object reference.
 *
 * \param property the property that will be modified.
 * \param value the new value of the property.
 */
TACTILE_IO_API
void AssignObject(Property& property, int32 id);

/**
 * \brief Sets the value of a property to a color.
 *
 * \param property the property that will be modified.
 * \param value the new value of the property.
 */
TACTILE_IO_API
void AssignColor(Property& property, const Color& color);

/**
 * \brief Returns the name of a property.
 *
 * \details By default, properties use the empty string as their name. However, note that
 * the empty string isn't a valid name in the Tactile editor, so make sure that all
 * properties are assigned names that are unique amongst their property siblings.
 *
 * \param property the property that will be queried.
 *
 * \return the current property name.
 */
TACTILE_IO_API_QUERY
auto GetName(const Property& property) -> CStr;

/**
 * \brief Returns the type of a property.
 *
 * \details By default, created properties hold (empty) string values.
 *
 * \param property the property that will be queried.
 *
 * \return the type of the property.
 */
TACTILE_IO_API_QUERY
auto GetType(const Property& property) -> PropertyType;

/**
 * \brief Returns the integer value of a property.
 *
 * \param property the property that will be queried.
 *
 * \return a signed integer value.
 *
 * \throws TactileError if the property did not hold an integer value.
 */
TACTILE_IO_API_QUERY
auto GetInt(const Property& property) -> int32;

/**
 * \brief Returns the float value of a property.
 *
 * \param property the property that will be queried.
 *
 * \return a float value.
 *
 * \throws TactileError if the property did not hold a float value.
 */
TACTILE_IO_API_QUERY
auto GetFloat(const Property& property) -> float;

/**
 * \brief Returns the string value of a property.
 *
 * \param property the property that will be queried.
 *
 * \return a pointer to the string value.
 *
 * \throws TactileError if the property did not hold a string value.
 */
TACTILE_IO_API_QUERY
auto GetString(const Property& property) -> CStr;

/**
 * \brief Returns the boolean value of a property.
 *
 * \param property the property that will be queried.
 *
 * \return a boolean value.
 *
 * \throws TactileError if the property did not hold a boolean value.
 */
TACTILE_IO_API_QUERY
auto GetBool(const Property& property) -> bool;

/**
 * \brief Returns the file path value of a property.
 *
 * \param property the property that will be queried.
 *
 * \return a pointer to the file path value.
 *
 * \throws TactileError if the property did not hold a file path value.
 */
TACTILE_IO_API_QUERY
auto GetFile(const Property& property) -> CPathStr;

/**
 * \brief Returns the object reference value of a property.
 *
 * \param property the property that will be queried.
 *
 * \return an object ID value.
 *
 * \throws TactileError if the property did not hold an object reference value.
 */
TACTILE_IO_API_QUERY
auto GetObject(const Property& property) -> int32;

/**
 * \brief Returns the color value of a property.
 *
 * \param property the property that will be queried.
 *
 * \return a color value.
 *
 * \throws TactileError if the property did not hold a color value.
 */
TACTILE_IO_API_QUERY
auto GetColor(const Property& property) -> Color;

/**
 * \brief Indicates whether or not a property holds an integer value.
 *
 * \param property the property that will be queried.
 *
 * \return `true` if the property holds an integer value; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsInt(const Property& property) -> bool;

/**
 * \brief Indicates whether or not a property holds a float value.
 *
 * \param property the property that will be queried.
 *
 * \return `true` if the property holds a float value; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsFloat(const Property& property) -> bool;

/**
 * \brief Indicates whether or not a property holds a string value.
 *
 * \param property the property that will be queried.
 *
 * \return `true` if the property holds a string value; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsString(const Property& property) -> bool;

/**
 * \brief Indicates whether or not a property holds a boolean value.
 *
 * \param property the property that will be queried.
 *
 * \return `true` if the property holds a boolean value; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsBool(const Property& property) -> bool;

/**
 * \brief Indicates whether or not a property holds a file path value.
 *
 * \param property the property that will be queried.
 *
 * \return `true` if the property holds a file path value; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsFile(const Property& property) -> bool;

/**
 * \brief Indicates whether or not a property holds an object reference value.
 *
 * \param property the property that will be queried.
 *
 * \return `true` if the property holds an object reference value; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsObject(const Property& property) -> bool;

/**
 * \brief Indicates whether or not a property holds a color value.
 *
 * \param property the property that will be queried.
 *
 * \return `true` if the property holds a color value; `false` otherwise.
 */
TACTILE_IO_API_QUERY
auto IsColor(const Property& property) -> bool;

/// \} End of property API

}  // namespace tactile::IO

#endif  // TACTILE_IO_IR_PROPERTY_HPP_
