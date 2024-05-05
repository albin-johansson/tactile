// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/i18n/language.hpp"

#include "tactile/core/ui/common/icons.hpp"

namespace tactile::ui {

Language::Language(const LanguageID id, Vector<String> strings)
  : mID {id},
    mStrings {std::move(strings)}
{}

void Language::add_icons()
{
  auto inject_icon = [this](const StringID string_id, const char* icon) {
    auto& string = mStrings[std::to_underlying(string_id)];
    string.insert(0, icon);
  };

  inject_icon(StringID::kPanUp, to_prefix_string(Icon::kMoveUp));
  inject_icon(StringID::kPanDown, to_prefix_string(Icon::kMoveDown));
  inject_icon(StringID::kPanLeft, to_prefix_string(Icon::kMoveLeft));
  inject_icon(StringID::kPanRight, to_prefix_string(Icon::kMoveRight));

  inject_icon(StringID::kCenterViewport, to_prefix_string(Icon::kAlignCenter));
  inject_icon(StringID::kIncreaseZoom, to_prefix_string(Icon::kZoomIn));
  inject_icon(StringID::kDecreaseZoom, to_prefix_string(Icon::kZoomOut));
  inject_icon(StringID::kResetZoom, to_prefix_string(Icon::kZoomReset));
  inject_icon(StringID::kToggleGrid, to_prefix_string(Icon::kGrid));

  inject_icon(StringID::kReportBug, to_prefix_string(Icon::kBug));
  inject_icon(StringID::kOpenDemoWindow, to_prefix_string(Icon::kToolbox));
  inject_icon(StringID::kOpenStyleEditor, to_prefix_string(Icon::kBrush));
}

auto Language::get_id() const -> LanguageID
{
  return mID;
}

}  // namespace tactile::ui
