#pragma once

#include <entt.hpp>  // dispatcher
#include <string>    // string

namespace Tactile {

class IDocument;

void UpdateRenamePropertyDialog(const IDocument& document,
                                entt::dispatcher& dispatcher);

void OpenRenamePropertyDialog(std::string name);

}  // namespace Tactile
