#pragma once

#include <QFormLayout>  // QFormLayout
#include <QWidget>      // QWidget

#include "forward_declare.hpp"

TACTILE_FORWARD_DECLARE(tactile::gui, color_preview_button)

namespace tactile::gui {

class color_preview_manager final : private QWidget
{
 public:
  explicit color_preview_manager(QFormLayout* layout);

  void update_preview(const QPalette& palette);

 private:
  color_preview_button* m_basePreview;
  color_preview_button* m_alternateBasePreview;
  color_preview_button* m_windowPreview;
  color_preview_button* m_windowTextPreview;
  color_preview_button* m_lightPreview;
  color_preview_button* m_midLightPreview;
  color_preview_button* m_darkPreview;
  color_preview_button* m_linkPreview;
  color_preview_button* m_linkVisitedPreview;
  color_preview_button* m_buttonPreview;
  color_preview_button* m_buttonTextPreview;
  color_preview_button* m_highlightPreview;
  color_preview_button* m_highlightedTextPreview;
  color_preview_button* m_placeholderTextPreview;
  color_preview_button* m_tooltipBasePreview;
  color_preview_button* m_tooltipTextPreview;
  color_preview_button* m_textPreview;
  color_preview_button* m_shadowPreview;
};

}  // namespace tactile::gui
