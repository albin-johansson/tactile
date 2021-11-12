#ifndef TACTILE_IO_IR_OBJECT_HPP_
#define TACTILE_IO_IR_OBJECT_HPP_

#include <tactile_def.hpp>

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \name Object API
/// \{

/**
 * \brief Sets the name of an object.
 *
 * \pre The supplied string cannot be null.
 *
 * \note This function will use a deep copy of the supplied string.
 *
 * \param object the object that will be modified.
 * \param name the new object name.
 */
TACTILE_IO_API void SetName(Object& object, CStr name);

/**
 * \brief Sets the unique ID associated with an object.
 *
 * \param object the object that will be modified.
 * \param id the new ID of the object.
 */
TACTILE_IO_API void SetId(Object& object, int32 id);

/**
 * \brief Sets the x-coordinate of an object.
 *
 * \details The default x-coordinate is zero.
 *
 * \param object the object that will be modified.
 * \param x the new x-coordinate of the object.
 */
TACTILE_IO_API void SetX(Object& object, float x);

/**
 * \brief Sets the y-coordinate of an object.
 *
 * \details The default y-coordinate is zero.
 *
 * \param object the object that will be modified.
 * \param y the new y-coordinate of the object.
 */
TACTILE_IO_API void SetY(Object& object, float y);

/**
 * \brief Sets the width of an object.
 *
 * \details The default width is zero.
 *
 * \param object the object that will be modified.
 * \param width the new width of the object.
 */
TACTILE_IO_API void SetWidth(Object& object, float width);

/**
 * \brief Sets the height of an object.
 *
 * \details The default height is zero.
 *
 * \param object the object that will be modified.
 * \param height the new height of the object.
 */
TACTILE_IO_API void SetHeight(Object& object, float height);

/**
 * \brief Sets the specific type of an object.
 *
 * \details Objects are by default considered to represent rectangles.
 *
 * \param object the object that will be modified.
 * \param type the new type of the object.
 */
TACTILE_IO_API void SetType(Object& object, ObjectType type);

/**
 * \brief Sets the tag of an object.
 *
 * \details Tags are entirely user-defined and any non-null string is valid.
 *
 * \pre The supplied string cannot be null.
 *
 * \note This function will use a deep copy of the supplied string.
 *
 * \param object the object that will be modified.
 * \param tag the new tag identifier.
 */
TACTILE_IO_API void SetTag(Object& object, CStr tag);

/**
 * \brief Sets the visibility of an object.
 *
 * \param object the object that will be modified.
 * \param visible `true` if the object should be visible; `false` otherwise.
 */
TACTILE_IO_API void SetVisible(Object& object, bool visible);

/**
 * \brief Reserves enough space for the specified amount of properties in an object.
 *
 * \details This function can be used to reduce the amount of internal reallocations
 * during consecutive calls to `AddProperty()`.
 *
 * \param object the object that will be affected.
 * \param n the number of properties to allocate enough space for.
 *
 * \see `AddProperty(Object&)`
 */
TACTILE_IO_API void ReserveProperties(Object& object, usize n);

/**
 * \brief Adds a property to an object.
 *
 * \param object the object that a property will be added to.
 *
 * \return a reference to the created property.
 *
 * \see `ReserveProperties(Object&)`
 */
TACTILE_IO_API_QUERY auto AddProperty(Object& object) -> Property&;

/**
 * \brief Returns the amount of properties associated with an object.
 *
 * \param object the object that will be queried.
 *
 * \return the amount of properties.
 */
TACTILE_IO_API_QUERY auto GetPropertyCount(const Object& object) -> usize;

/**
 * \brief Returns the property at the specified index in an object.
 *
 * \details This function will throw if there is no property associated with the index.
 *
 * \param object the object that will be queried.
 * \param index the index of the desired property.
 *
 * \return a reference to the property at the specified index.
 */
TACTILE_IO_API_QUERY auto GetProperty(Object& object, usize index) -> Property&;

/// \copydoc GetProperty()
TACTILE_IO_API_QUERY auto GetProperty(const Object& object, usize index)
    -> const Property&;

/**
 * \brief Returns the name of an object.
 *
 * \param object the object that will be queried.
 *
 * \return the object name.
 */
TACTILE_IO_API_QUERY auto GetName(const Object& object) -> CStr;

/**
 * \brief Returns the unique ID associated with an object.
 *
 * \param object the object that will be queried.
 *
 * \return the object ID.
 */
TACTILE_IO_API_QUERY auto GetId(const Object& object) -> int32;

/**
 * \brief Returns the x-coordinate of an object.
 *
 * \param object the object that will be queried.
 *
 * \return an x-coordinate.
 */
TACTILE_IO_API_QUERY auto GetX(const Object& object) -> float;

/**
 * \brief Returns the y-coordinate of an object.
 *
 * \param object the object that will be queried.
 *
 * \return a y-coordinate.
 */
TACTILE_IO_API_QUERY auto GetY(const Object& object) -> float;

/**
 * \brief Returns the width of an object.
 *
 * \param object the object that will be queried.
 *
 * \return the object width.
 */
TACTILE_IO_API_QUERY auto GetWidth(const Object& object) -> float;

/**
 * \brief Returns the height of an object.
 *
 * \param object the object that will be queried.
 *
 * \return the object height.
 */
TACTILE_IO_API_QUERY auto GetHeight(const Object& object) -> float;

/**
 * \brief Returns the type of an object.
 *
 * \param object the object that will be queried.
 *
 * \return the object type.
 */
TACTILE_IO_API_QUERY auto GetType(const Object& object) -> ObjectType;

/**
 * \brief Returns the user-defined tag of an object.
 *
 * \param object the object that will queried.
 *
 * \return the object tag string.
 */
TACTILE_IO_API_QUERY auto GetTag(const Object& object) -> CStr;

/**
 * \brief Indicates whether or not an object is visible.
 *
 * \param object the object that will be queried.
 *
 * \return `true` if the object is visible; `false` otherwise.
 */
TACTILE_IO_API_QUERY auto IsVisible(const Object& object) -> bool;

/// \} End of Object API

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_OBJECT_HPP_
