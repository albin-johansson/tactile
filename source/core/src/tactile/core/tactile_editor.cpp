// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tactile_editor.hpp"

#include "tactile/core/debug/validation.hpp"
#include "tactile/render/window.hpp"

namespace tactile {

TactileEditor::TactileEditor(IWindow* window)
  : m_window {require_not_null(window, "null window")}
{}

void TactileEditor::on_startup()
{
  m_window->show();
}

void TactileEditor::on_shutdown()
{
  m_window->hide();
}

void TactileEditor::on_update()
{
}

void TactileEditor::on_framebuffer_scale_changed(const float framebuffer_scale)
{}

}  // namespace tactile
