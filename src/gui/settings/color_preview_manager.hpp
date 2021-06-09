#pragma once

#include <QFormLayout>  // QFormLayout
#include <QPalette>     // QPalette
#include <QWidget>      // QWidget

#include "forward_declare.hpp"

TACTILE_FORWARD_DECLARE(tactile, ColorPreviewButton)

namespace tactile::gui {

class color_preview_manager final : public QWidget
{
  Q_OBJECT

 public:
  explicit color_preview_manager(QFormLayout* layout,
                                 QPalette::ColorGroup group,
                                 QWidget* parent = nullptr);

  void update_preview(const QPalette& palette);

 signals:
  void color_changed(QPalette::ColorRole role, const QColor& color);

 private:
  QPalette::ColorGroup m_group;
  ColorPreviewButton* m_basePreview;
  ColorPreviewButton* m_alternateBasePreview;
  ColorPreviewButton* m_windowPreview;
  ColorPreviewButton* m_windowTextPreview;
  ColorPreviewButton* m_lightPreview;
  ColorPreviewButton* m_midLightPreview;
  ColorPreviewButton* m_darkPreview;
  ColorPreviewButton* m_linkPreview;
  ColorPreviewButton* m_linkVisitedPreview;
  ColorPreviewButton* m_buttonPreview;
  ColorPreviewButton* m_buttonTextPreview;
  ColorPreviewButton* m_highlightPreview;
  ColorPreviewButton* m_highlightedTextPreview;
  ColorPreviewButton* m_placeholderTextPreview;
  ColorPreviewButton* m_tooltipBasePreview;
  ColorPreviewButton* m_tooltipTextPreview;
  ColorPreviewButton* m_textPreview;
  ColorPreviewButton* m_shadowPreview;
};

}  // namespace tactile::gui
