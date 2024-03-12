// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

struct ImGuiInputTextCallbackData;

namespace tactile::ui {

/// A basic filter that only allows a basic subset of characters that
/// should guarantee use of names that are usable as file names.
[[nodiscard]] auto filename_filter(ImGuiInputTextCallbackData* data) -> int;

}  // namespace tactile::ui
