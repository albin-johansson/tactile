#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Model;

void MapContentViewport(const Model& model, entt::dispatcher& dispatcher);

void CenterMapContentViewport();

void SetMapContentViewportGridEnabled(bool enabled);

}  // namespace tactile
