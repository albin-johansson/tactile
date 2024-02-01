// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/windows.hpp"

#include <imgui.h>

#include "tactile/foundation/debug/validation.hpp"

namespace tactile::core {

ScopedWindow::ScopedWindow(const char* title, const ScopedWindowOptions& options)
  : mIsOpen {
      ImGui::Begin(require_not_null(title, "null title"), options.is_open, options.flags)}
{}

ScopedWindow::~ScopedWindow() noexcept
{
  ImGui::End();
}

auto ScopedWindow::is_open() const -> bool
{
  return mIsOpen;
}

}  // namespace tactile::core
