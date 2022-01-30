#pragma once

#include <centurion.hpp>
#include <imgui.h>

#include "core/region.hpp"
#include "tactile_def.hpp"

namespace tactile {

struct RenderInfo;

/**
 * \brief Provides a common simplified rendering API.
 */
class Graphics final {
 public:
  explicit Graphics(const RenderInfo& info);

  void PushClip();

  void PopClip();

  void Clear();

  void DrawRect(const ImVec2& position, const ImVec2& size);

  void FillRect(const ImVec2& position, const ImVec2& size);

  void DrawTranslatedRect(const ImVec2& position, const ImVec2& size);

  void FillTranslatedRect(const ImVec2& position, const ImVec2& size);

  void DrawRectWithShadow(const ImVec2& position, const ImVec2& size);

  void DrawTranslatedRectWithShadow(const ImVec2& position, const ImVec2& size);

  void DrawCircleWithShadow(const ImVec2& center, float radius);

  void DrawTranslatedCircleWithShadow(const ImVec2& center, float radius);

  void DrawEllipseWithShadow(const ImVec2& center, const ImVec2& radius);

  void DrawTranslatedEllipseWithShadow(const ImVec2& center, const ImVec2& radius);

  void RenderImage(uint texture, const ImVec2& position, const ImVec2& size);

  /**
   * \brief Renders a portion of a tileset texture.
   *
   * \param texture the identifier for the source texture that will be rendered.
   * \param source the region of the texture that will be rendered.
   * \param position the position of the rendered image on the screen.
   * \param uv the ratio between the tileset tile size and the texture size.
   */
  void RenderImage(uint texture,
                   const ImVec4& source,
                   const ImVec2& position,
                   const ImVec2& uv);

  void RenderTranslatedImage(uint texture,
                             const ImVec4& source,
                             const ImVec2& position,
                             const ImVec2& uv);

  void RenderText(CStr text, const ImVec2& position);

  void RenderTranslatedText(CStr text, const ImVec2& position);

  void RenderCenteredText(CStr text, const ImVec2& center);

  void RenderTranslatedGrid();

  void SetDrawColor(const cen::color& color);

  void SetOpacity(float opacity);

  void SetLineThickness(float thickness);

  [[nodiscard]] auto FromMatrixToAbsolute(int32 row, int32 column) const -> ImVec2;

  [[nodiscard]] auto IsIntersectingBounds(const ImVec2& position,
                                          const ImVec2& size) const -> bool;

  [[nodiscard]] auto IsWithinTranslatedBounds(const ImVec2& position) const -> bool;

  [[nodiscard]] auto Translate(const ImVec2& position) const -> ImVec2;

  [[nodiscard]] auto GetOrigin() const -> ImVec2 { return mOrigin; }

  [[nodiscard]] auto GetViewportTileSize() const -> ImVec2 { return mViewportTileSize; }

  [[nodiscard]] auto GetLogicalTileSize() const -> ImVec2 { return mLogicalTileSize; }

  [[nodiscard]] auto GetTileSizeRatio() const -> ImVec2 { return mTileSizeRatio; }

  [[nodiscard]] auto GetBounds() const -> Region { return mBounds; }

 private:
  ImVec2 mCanvasTL;
  ImVec2 mCanvasBR;
  ImVec2 mOrigin{0, 0};
  ImVec2 mViewportTileSize{32, 32};
  ImVec2 mLogicalTileSize{32, 32};
  ImVec2 mTileSizeRatio{1, 1};
  Region mBounds;
  cen::frect mBoundsRect;
  cen::color mDrawColor{cen::colors::black};
  float mLineThickness{1};
  uint8 mOpacity{255};

  [[nodiscard]] auto GetDrawColor() const -> uint32;

  [[nodiscard]] auto GetShadowDrawColor() const -> uint32;
};

}  // namespace tactile