#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class IDocument;

void UpdateAddPropertyDialog(const IDocument& document,
                             entt::dispatcher& dispatcher);

void OpenAddPropertyDialog();

}  // namespace Tactile
