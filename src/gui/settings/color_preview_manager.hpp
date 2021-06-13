#pragma once

#include <QFormLayout>  // QFormLayout
#include <QPalette>     // QPalette
#include <QWidget>      // QWidget

namespace tactile {

class ColorPreviewButton;

class ColorPreviewManager final : public QWidget
{
  Q_OBJECT

 public:
  explicit ColorPreviewManager(QFormLayout* layout,
                               QPalette::ColorGroup group,
                               QWidget* parent = nullptr);

  void UpdatePreview(const QPalette& palette);

 signals:
  void S_ColorChanged(QPalette::ColorRole role, const QColor& color);

 private:
  QPalette::ColorGroup mGroup;
  ColorPreviewButton* mBasePreview;
  ColorPreviewButton* mAlternateBasePreview;
  ColorPreviewButton* mWindowPreview;
  ColorPreviewButton* mWindowTextPreview;
  ColorPreviewButton* mLightPreview;
  ColorPreviewButton* mMidLightPreview;
  ColorPreviewButton* mDarkPreview;
  ColorPreviewButton* mLinkPreview;
  ColorPreviewButton* mLinkVisitedPreview;
  ColorPreviewButton* mButtonPreview;
  ColorPreviewButton* mButtonTextPreview;
  ColorPreviewButton* mHighlightPreview;
  ColorPreviewButton* mHighlightedTextPreview;
  ColorPreviewButton* mPlaceholderTextPreview;
  ColorPreviewButton* mTooltipBasePreview;
  ColorPreviewButton* mTooltipTextPreview;
  ColorPreviewButton* mTextPreview;
  ColorPreviewButton* mShadowPreview;

  void ConnectPreview(ColorPreviewButton* button, QPalette::ColorRole role);
};

}  // namespace tactile
