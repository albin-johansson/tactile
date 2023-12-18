// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/fonts.hpp"

#include <IconsFontAwesome6.h>
#include <imgui.h>

#include "tactile/foundation/container/array.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/render/renderer.hpp"

namespace tactile {
namespace {

inline constexpr Array<ImWchar, 3> kFontIconRange {ICON_MIN_FA, ICON_MAX_FA, 0};

}  // namespace

void try_reload_imgui_fonts(IRenderer& renderer,
                            const int font_size,
                            const float framebuffer_scale)
{
  if (renderer.can_reload_fonts_texture()) {
    TACTILE_LOG_TRACE("Reloading fonts...");

    auto& io = ImGui::GetIO();
    io.Fonts->Clear();

    const auto scaled_font_size = static_cast<float>(font_size) * framebuffer_scale;

    ImFontConfig roboto_config {};
    roboto_config.SizePixels = scaled_font_size;
    io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf",
                                 roboto_config.SizePixels,
                                 &roboto_config);

    // The global scale is 1 on most platforms, and 0.5 on macOS.
    io.FontGlobalScale = 1.0f / framebuffer_scale;

    ImFontConfig icon_config {};
    icon_config.MergeMode = true;
    icon_config.SizePixels = scaled_font_size;
    icon_config.GlyphMinAdvanceX = icon_config.SizePixels;
    icon_config.GlyphMaxAdvanceX = icon_config.GlyphMinAdvanceX;
    icon_config.GlyphOffset = {0, 2};
    io.Fonts->AddFontFromFileTTF("assets/fonts/fa/" FONT_ICON_FILE_NAME_FAS,
                                 icon_config.GlyphMinAdvanceX,
                                 &icon_config,
                                 kFontIconRange.data());

    io.Fonts->Build();

    renderer.reload_fonts_texture();

    ImGui::GetStyle().ScaleAllSizes(1.0f);
  }
}

}  // namespace tactile
