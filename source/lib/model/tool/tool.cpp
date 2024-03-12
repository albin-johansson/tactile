// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "model/tool/tool.hpp"

namespace tactile {

void Tool::on_enabled(DocumentModel&, entt::dispatcher&) {}

void Tool::on_disabled(DocumentModel&, entt::dispatcher&) {}

void Tool::on_entered(DocumentModel&, entt::dispatcher&) {}

void Tool::on_exited(DocumentModel&, entt::dispatcher&) {}

void Tool::on_pressed(DocumentModel&, entt::dispatcher&, const MouseInfo&) {}

void Tool::on_dragged(DocumentModel&, entt::dispatcher&, const MouseInfo&) {}

void Tool::on_released(DocumentModel&, entt::dispatcher&, const MouseInfo&) {}

auto Tool::is_available(const DocumentModel&) const -> bool
{
  return true;
}

}  // namespace tactile
