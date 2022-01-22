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

/**
 * \brief Calls the supplied callable for each tileset provided by the map.
 *
 * \tparam U the type of the function object.
 *
 * \param map the map that provides the layers to be visited.
 * \param callable the function object that is invoked for each layer.
 */
template <typename U>
void EachTileset(const Map& map, U&& callable)
{
  const auto count = GetTilesetCount(map);
  for (usize index = 0; index < count; ++index) {
    const auto& tileset = GetTileset(map, index);
    callable(tileset);
  }
}

/**
 * \brief Calls the supplied callable for each tile info object provided by the tileset.
 *
 * \note Do not confuse the notion of tile info objects and the individual tiles
 * associated with a tileset.
 *
 * \tparam U the type of the function object.
 *
 * \param tileset the tileset that provides the tile info objects to be visited.
 * \param callable the function object that is invoked for each tile info.
 */
template <typename U>
void EachTileInfo(const Tileset& tileset, U&& callable)
{
  const auto count = GetTileInfoCount(tileset);
  for (usize index = 0; index < count; ++index) {
    const auto& tile = GetTileInfo(tileset, index);
    callable(tile);
  }
}

template <typename U>
void EachAnimationFrame(const Tile& tile, U&& callable)
{
  const auto count = GetAnimationFrameCount(tile);
  for (usize index = 0; index < count; ++index) {
    const auto& frame = GetAnimationFrame(tile, index);
    callable(frame);
  }
}

/**
 * \brief Calls the supplied callable for each layer provided by the source object.
 *
 * \tparam T the type of the source object.
 * \tparam U the type of the function object.
 *
 * \param source the source object that provides the layers to be visited.
 * \param callable the function object that is invoked for each layer.
 */
template <typename T, typename U>
void EachLayer(const T& source, U&& callable)
{
  const auto count = GetLayerCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& layer = GetLayer(source, index);
    callable(layer);
  }
}

/**
 * \brief Calls the supplied callable for each map object provided by the source object.
 *
 * \tparam T the type of the source object.
 * \tparam U the type of the function object.
 *
 * \param source the source object that provides the map objects to be visited.
 * \param callable the function object that is invoked for each map object.
 */
template <typename T, typename U>
void EachObject(const T& source, U&& callable)
{
  const auto count = GetObjectCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& object = GetObject(source, index);
    callable(object);
  }
}

/**
 * \brief Calls the supplied callable for each property provided by the source object.
 *
 * \tparam T the type of the source object.
 * \tparam U the type of the function object.
 *
 * \param source the source object that provides the properties to be visited.
 * \param callable the function object that is invoked for each property.
 */
template <typename T, typename U>
void EachProperty(const T& source, U&& callable)
{
  const auto count = GetPropertyCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& property = GetProperty(source, index);
    callable(property);
  }
}

template <typename U>
void EachComponentDef(const Map& map, U&& callable)
{
  const auto count = GetComponentDefCount(map);
  for (usize index = 0; index < count; ++index) {
    const auto& def = GetComponentDef(map, index);
    callable(def);
  }
}

template <typename U>
void EachAttribute(const ComponentDef& def, U&& callable)
{
  const auto count = GetAttributeCount(def);
  for (usize index = 0; index < count; ++index) {
    const auto* name = GetAttributeName(def, index);
    callable(name);
  }
}

template <typename U>
void EachAttribute(const Component& component, U&& callable)
{
  const auto count = GetAttributeCount(component);
  for (usize index = 0; index < count; ++index) {
    const auto* name = GetAttributeName(component, index);
    callable(name);
  }
}

template <typename T, typename U>
void EachComponent(const T& source, U&& callable)
{
  const auto count = GetComponentCount(source);
  for (usize index = 0; index < count; ++index) {
    const auto& component = GetComponent(source, index);
    callable(component);
  }
}

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_HELPERS_HPP_
