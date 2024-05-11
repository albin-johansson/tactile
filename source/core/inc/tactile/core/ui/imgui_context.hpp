// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"

struct ImGuiContext;

namespace tactile::ui {

struct ImGuiContextDeleter final
{
  void operator()(ImGuiContext* context) noexcept;
};

using UniqueImGuiContext = Unique<ImGuiContext, ImGuiContextDeleter>;

}  // namespace tactile::ui
