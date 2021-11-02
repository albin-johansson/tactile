#ifndef TACTILE_IO_IR_OBJECT_HPP_
#define TACTILE_IO_IR_OBJECT_HPP_

#include <tactile-base/object_type.hpp>

#include "api.hpp"
#include "ir_common.hpp"

namespace Tactile::IO {

/// \name Object API
/// \{

TACTILE_IO_API void SetName(Object& object, CStr name);
TACTILE_IO_API void SetId(Object& object, int32 id);
TACTILE_IO_API void SetX(Object& object, float x);
TACTILE_IO_API void SetY(Object& object, float y);
TACTILE_IO_API void SetWidth(Object& object, float width);
TACTILE_IO_API void SetHeight(Object& object, float height);
TACTILE_IO_API void SetType(Object& object, ObjectType type);
TACTILE_IO_API void SetTag(Object& object, CStr tag);
TACTILE_IO_API void SetVisible(Object& object, bool visible);

TACTILE_IO_API void ReserveProperties(Object& object, usize n);
TACTILE_IO_API_QUERY auto AddProperty(Object& object) -> Property&;

TACTILE_IO_API_QUERY auto GetPropertyCount(const Object& object) -> usize;
TACTILE_IO_API_QUERY auto GetProperty(const Object& object, usize index)
    -> const Property&;

TACTILE_IO_API_QUERY auto GetName(const Object& object) -> CStr;
TACTILE_IO_API_QUERY auto GetId(const Object& object) -> int32;
TACTILE_IO_API_QUERY auto GetX(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetY(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetWidth(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetHeight(const Object& object) -> float;
TACTILE_IO_API_QUERY auto GetType(const Object& object) -> ObjectType;
TACTILE_IO_API_QUERY auto GetTag(const Object& object) -> CStr;
TACTILE_IO_API_QUERY auto IsVisible(const Object& object) -> bool;

/// \} End of Object API

}  // namespace Tactile::IO

#endif  // TACTILE_IO_IR_OBJECT_HPP_
