#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace tactile {

class document_model;

void UpdateDocumentTabs(const document_model& model, entt::dispatcher& dispatcher);

void CenterMapViewport();

}  // namespace tactile
