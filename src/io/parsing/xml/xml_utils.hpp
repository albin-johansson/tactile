#pragma once

#include <pugixml.hpp>  // xml_node
#include <string>       // string
#include <vector>       // vector

#include "aliases/czstring.hpp"
#include "aliases/maybe.hpp"
#include "aliases/not_null.hpp"

namespace Tactile::IO {

[[nodiscard]] auto Collect(pugi::xml_node node, czstring str)
    -> std::vector<pugi::xml_node>;

[[nodiscard]] auto Contains(pugi::xml_node node, NotNull<czstring> attr) -> bool;

[[nodiscard]] auto GetString(pugi::xml_node node, NotNull<czstring> attr)
    -> Maybe<std::string>;

[[nodiscard]] auto GetInt(pugi::xml_node node, NotNull<czstring> attr) -> Maybe<int>;

[[nodiscard]] auto GetFloat(pugi::xml_node node, NotNull<czstring> attr)
    -> Maybe<float>;

[[nodiscard]] auto GetBool(pugi::xml_node node, NotNull<czstring> attr)
    -> Maybe<bool>;

}  // namespace Tactile::IO
