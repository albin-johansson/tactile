#ifndef TACTILE_IO_IR_PROPERTY_HPP_
#define TACTILE_IO_IR_PROPERTY_HPP_

#include <tactile-base/property_type.hpp>

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \name Property API
/// \{

TACTILE_IO_API void SetName(Property& property, CStr name);
TACTILE_IO_API void AssignInt(Property& property, int32 value);
TACTILE_IO_API void AssignFloat(Property& property, float value);
TACTILE_IO_API void AssignString(Property& property, CStr str);
TACTILE_IO_API void AssignBool(Property& property, bool value);
TACTILE_IO_API void AssignFile(Property& property, CPathStr path);
TACTILE_IO_API void AssignObject(Property& property, int32 id);
TACTILE_IO_API void AssignColor(Property& property, const Color& color);

TACTILE_IO_API_QUERY auto GetName(const Property& property) -> CStr;
TACTILE_IO_API_QUERY auto GetType(const Property& property) -> PropertyType;
TACTILE_IO_API_QUERY auto GetInt(const Property& property) -> int32;
TACTILE_IO_API_QUERY auto GetFloat(const Property& property) -> float;
TACTILE_IO_API_QUERY auto GetString(const Property& property) -> CStr;
TACTILE_IO_API_QUERY auto GetBool(const Property& property) -> bool;
TACTILE_IO_API_QUERY auto GetFile(const Property& property) -> CPathStr;
TACTILE_IO_API_QUERY auto GetObject(const Property& property) -> int32;
TACTILE_IO_API_QUERY auto GetColor(const Property& property) -> Color;

/// \} End of property API

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_PROPERTY_HPP_
