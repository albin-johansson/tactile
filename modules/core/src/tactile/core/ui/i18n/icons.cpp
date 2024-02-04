// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/i18n/icons.hpp"

#include <utility>  // to_underlying

#include <IconsFontAwesome6.h>

#include "tactile/core/ui/i18n/string_id.hpp"
#include "tactile/foundation/debug/exception.hpp"

namespace tactile::core {

void inject_icons(Vector<String>& strings)
{
  if (strings.size() != std::to_underlying(StringID::kMAX)) {
    throw Exception {"invalid number of translated strings"};
  }

  auto inject_icon = [&](const StringID string_id, const char* icon) {
    auto& string = strings.at(std::to_underlying(string_id));
    string.insert(0, icon);
  };

  inject_icon(StringID::kPanUp, ICON_FA_ARROW_UP " ");
  inject_icon(StringID::kPanDown, ICON_FA_ARROW_DOWN " ");
  inject_icon(StringID::kPanLeft, ICON_FA_ARROW_LEFT " ");
  inject_icon(StringID::kPanRight, ICON_FA_ARROW_RIGHT " ");

  inject_icon(StringID::kCenterViewport, ICON_FA_CROSSHAIRS " ");
  inject_icon(StringID::kIncreaseZoom, ICON_FA_MAGNIFYING_GLASS_PLUS " ");
  inject_icon(StringID::kDecreaseZoom, ICON_FA_MAGNIFYING_GLASS_MINUS " ");
  inject_icon(StringID::kResetZoom, ICON_FA_MAGNIFYING_GLASS " ");
  inject_icon(StringID::kToggleGrid, ICON_FA_TABLE_CELLS " ");

  inject_icon(StringID::kReportBug, ICON_FA_BUG " ");
  inject_icon(StringID::kOpenDemoWindow, ICON_FA_TOOLBOX " ");
  inject_icon(StringID::kOpenStyleEditor, ICON_FA_BRUSH " ");
}

auto get_icon_string(const IconID icon_id) -> const char*
{
  switch (icon_id) {
    case IconID::kPlus: return ICON_FA_CIRCLE_PLUS;
    case IconID::kMinus: return ICON_FA_CIRCLE_MINUS;
    case IconID::kClone: return ICON_FA_CLONE;
    case IconID::kUpArrow: return ICON_FA_ARROW_UP;
    case IconID::kDownArrow: return ICON_FA_ARROW_DOWN;
    default: throw Exception {"invalid icon identifier"};
  }
}

}  // namespace tactile::core
