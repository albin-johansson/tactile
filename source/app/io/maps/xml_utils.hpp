#pragma once

#include <vector>  // vector

#include <pugixml.hpp>

#include "tactile_def.hpp"

namespace tactile {

[[nodiscard]] auto collect_children(pugi::xml_node source, const char* name)
    -> std::vector<pugi::xml_node>;

[[nodiscard]] auto has_attribute(pugi::xml_node node, const char* attributeName) -> bool;

[[nodiscard]] auto string_attribute(pugi::xml_node node, const char* attributeName)
    -> maybe<std::string>;

[[nodiscard]] auto int_attribute(pugi::xml_node node, const char* attributeName)
    -> maybe<int32>;

[[nodiscard]] auto uint_attribute(pugi::xml_node node, const char* attributeName)
    -> maybe<uint32>;

[[nodiscard]] auto float_attribute(pugi::xml_node node, const char* attributeName)
    -> maybe<float>;

[[nodiscard]] auto bool_attribute(pugi::xml_node node, const char* attributeName)
    -> maybe<bool>;

}  // namespace tactile
