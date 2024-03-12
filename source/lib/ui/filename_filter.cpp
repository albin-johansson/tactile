// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "filename_filter.hpp"

#include <locale>  // locale, isalpha, isdigit, isspace

#include <imgui.h>

namespace tactile::ui {

auto filename_filter(ImGuiInputTextCallbackData* data) -> int
{
  const auto& locale = std::locale::classic();
  const auto ch = static_cast<wchar_t>(data->EventChar);

  if (std::isalpha(ch, locale) || std::isdigit(ch, locale) || std::isspace(ch, locale) ||
      ch == '-' || ch == '_') {
    return 0;  // Accept the character
  }
  else {
    return 1;  // Reject the character
  }
}

}  // namespace tactile::ui
