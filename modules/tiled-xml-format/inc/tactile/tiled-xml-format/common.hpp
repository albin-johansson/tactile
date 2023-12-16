// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // forward

#include <pugixml.hpp>

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_string(pugi::xml_node node, const char* key)
    -> Result<String>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_int(pugi::xml_node node, const char* key)
    -> Result<int>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_uint(pugi::xml_node node, const char* key)
    -> Result<uint>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_float(pugi::xml_node node, const char* key)
    -> Result<float>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_bool(pugi::xml_node node, const char* key)
    -> Result<bool>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, String& value)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, int& value)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, uint& value)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, usize& value)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, float& value)
    -> Result<void>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto parse_to(pugi::xml_node node, const char* key, bool& value)
    -> Result<void>;

template <typename T>
[[nodiscard]] auto try_parse_to(const pugi::xml_node node,
                                const char* key,
                                T& value,
                                T&& default_value = T{}) -> Result<void>
{
  if (!node.attribute(key).empty()) {
    return parse_to(node, key, value);
  }
  else {
    value = std::forward<T>(default_value);
    return kOK;
  }
}

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto load_xml_file(const FilePath& path)
    -> Result<pugi::xml_document>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto save_xml_file(const pugi::xml_document& document,
                                          const FilePath& path,
                                          int indentation) -> Result<void>;

}  // namespace tactile::tiled::tmx
