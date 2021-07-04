#pragma once

#include <entt.hpp>  // dispatcher
#include <string>    // string

namespace Tactile {

class IDocument;

void UpdateChangePropertyTypeDialog(const IDocument& document,
                                    entt::dispatcher& dispatcher);

void OpenChangePropertyTypeDialog(std::string name);

}  // namespace Tactile
