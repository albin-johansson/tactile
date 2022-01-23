#ifndef TACTILE_IO_IR_COMPONENT_HPP_
#define TACTILE_IO_IR_COMPONENT_HPP_

#include <tactile_def.hpp>

#include "api.hpp"
#include "ir_common.hpp"

namespace tactile::IO {

/// \name Component API
/// \{

TACTILE_IO_API
auto DefineComponent(Map& map, CStr name) -> ComponentDef&;

TACTILE_IO_API
void DefineAttribute(ComponentDef& def, CStr name, PropertyType type);

TACTILE_IO_API
void SetAttributeDefaultString(ComponentDef& def, CStr attribute, CStr str);

TACTILE_IO_API
void SetAttributeDefaultFile(ComponentDef& def, CStr attribute, CPathStr path);

TACTILE_IO_API
void SetAttributeDefaultInt(ComponentDef& def, CStr attribute, int32 value);

TACTILE_IO_API
void SetAttributeDefaultObject(ComponentDef& def, CStr attribute, int32 id);

TACTILE_IO_API
void SetAttributeDefaultFloat(ComponentDef& def, CStr attribute, float value);

TACTILE_IO_API
void SetAttributeDefaultBool(ComponentDef& def, CStr attribute, bool value);

TACTILE_IO_API
void SetAttributeDefaultColor(ComponentDef& def, CStr attribute, Color value);

TACTILE_IO_API_QUERY
auto GetAttributeType(const ComponentDef& def, CStr attribute) -> PropertyType;

TACTILE_IO_API_QUERY
auto GetAttributeType(const Component& component, CStr attribute) -> PropertyType;

TACTILE_IO_API_QUERY
auto IsAttributeString(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto IsAttributeFile(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto IsAttributeInt(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto IsAttributeFloat(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto IsAttributeBool(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto IsAttributeColor(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto IsAttributeObject(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API
void SetName(ComponentDef& def, CStr name);

TACTILE_IO_API_QUERY
auto GetName(const ComponentDef& def) -> CStr;

TACTILE_IO_API_QUERY
auto GetName(const Component& component) -> CStr;

TACTILE_IO_API_QUERY
auto GetAttributeName(const ComponentDef& def, usize index) -> CStr;

TACTILE_IO_API_QUERY
auto GetAttributeName(const Component& component, usize index) -> CStr;

TACTILE_IO_API_QUERY
auto GetAttributeCount(const ComponentDef& def) -> usize;

TACTILE_IO_API_QUERY
auto GetAttributeCount(const Component& component) -> usize;

TACTILE_IO_API_QUERY
auto HasDefaultValue(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto GetComponentDef(Map& map, CStr name) -> ComponentDef&;

TACTILE_IO_API_QUERY
auto GetComponentDef(const Map& map, CStr name) -> const ComponentDef&;

TACTILE_IO_API_QUERY
auto GetComponentDef(const Map& map, usize index) -> const ComponentDef&;

TACTILE_IO_API_QUERY
auto GetComponentDefCount(const Map& map) -> usize;

TACTILE_IO_API
void ReserveComponentDefinitions(Map& map, usize n);

TACTILE_IO_API
void ReserveComponents(Map& map, usize n);

TACTILE_IO_API
void ReserveComponents(Tileset& tileset, usize n);

TACTILE_IO_API
void ReserveComponents(Layer& layer, usize n);

TACTILE_IO_API
void ReserveComponents(Object& object, usize n);

TACTILE_IO_API
void ReserveComponents(Tile& tile, usize n);

TACTILE_IO_API
auto AddComponent(Map& map, const ComponentDef& def) -> Component&;

TACTILE_IO_API
auto AddComponent(Tileset& tileset, const ComponentDef& def) -> Component&;

TACTILE_IO_API
auto AddComponent(Layer& layer, const ComponentDef& def) -> Component&;

TACTILE_IO_API
auto AddComponent(Object& object, const ComponentDef& def) -> Component&;

TACTILE_IO_API
auto AddComponent(Tile& tile, const ComponentDef& def) -> Component&;

TACTILE_IO_API
void SetAttributeString(Component& component, CStr attribute, CStr value);

TACTILE_IO_API
void SetAttributeFile(Component& component, CStr attribute, CPathStr value);

TACTILE_IO_API
void SetAttributeInt(Component& component, CStr attribute, int32 value);

TACTILE_IO_API
void SetAttributeFloat(Component& component, CStr attribute, float value);

TACTILE_IO_API
void SetAttributeBool(Component& component, CStr attribute, bool value);

TACTILE_IO_API
void SetAttributeColor(Component& component, CStr attribute, const Color& value);

TACTILE_IO_API
void SetAttributeObject(Component& component, CStr attribute, int32 id);

TACTILE_IO_API_QUERY
auto GetInt(const ComponentDef& def, CStr attribute) -> int32;

TACTILE_IO_API_QUERY
auto GetInt(const Component& component, CStr attribute) -> int32;

TACTILE_IO_API_QUERY
auto GetFloat(const ComponentDef& def, CStr attribute) -> float;

TACTILE_IO_API_QUERY
auto GetFloat(const Component& component, CStr attribute) -> float;

TACTILE_IO_API_QUERY
auto GetBool(const ComponentDef& def, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto GetBool(const Component& component, CStr attribute) -> bool;

TACTILE_IO_API_QUERY
auto GetString(const ComponentDef& def, CStr attribute) -> CStr;

TACTILE_IO_API_QUERY
auto GetString(const Component& component, CStr attribute) -> CStr;

TACTILE_IO_API_QUERY
auto GetFile(const ComponentDef& def, CStr attribute) -> CPathStr;

TACTILE_IO_API_QUERY
auto GetFile(const Component& component, CStr attribute) -> CPathStr;

TACTILE_IO_API_QUERY
auto GetColor(const ComponentDef& def, CStr attribute) -> Color;

TACTILE_IO_API_QUERY
auto GetColor(const Component& component, CStr attribute) -> Color;

TACTILE_IO_API_QUERY
auto GetObject(const ComponentDef& def, CStr attribute) -> int32;

TACTILE_IO_API_QUERY
auto GetObject(const Component& component, CStr attribute) -> int32;

TACTILE_IO_API_QUERY
auto GetComponent(const Map& map, usize index) -> const Component&;

TACTILE_IO_API_QUERY
auto GetComponent(const Tileset& tileset, usize index) -> const Component&;

TACTILE_IO_API_QUERY
auto GetComponent(const Layer& layer, usize index) -> const Component&;

TACTILE_IO_API_QUERY
auto GetComponent(const Object& object, usize index) -> const Component&;

TACTILE_IO_API_QUERY
auto GetComponent(const Tile& tile, usize index) -> const Component&;

TACTILE_IO_API_QUERY
auto GetComponentCount(const Map& map) -> usize;

TACTILE_IO_API_QUERY
auto GetComponentCount(const Tileset& tileset) -> usize;

TACTILE_IO_API_QUERY
auto GetComponentCount(const Layer& layer) -> usize;

TACTILE_IO_API_QUERY
auto GetComponentCount(const Object& object) -> usize;

TACTILE_IO_API_QUERY
auto GetComponentCount(const Tile& tile) -> usize;

/// \} End of component API

}  // namespace tactile::IO

#endif  // TACTILE_IO_IR_COMPONENT_HPP_