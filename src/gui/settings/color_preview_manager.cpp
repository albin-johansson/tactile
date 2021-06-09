#include "color_preview_manager.hpp"

#include "color_preview_button.hpp"

namespace tactile {

ColorPreviewManager::ColorPreviewManager(QFormLayout* layout,
                                         const QPalette::ColorGroup group,
                                         QWidget* parent)
    : QWidget{parent}
    , mGroup{group}
    , mBasePreview{new ColorPreviewButton{Qt::black, this}}
    , mAlternateBasePreview{new ColorPreviewButton{Qt::black, this}}
    , mWindowPreview{new ColorPreviewButton{Qt::black, this}}
    , mWindowTextPreview{new ColorPreviewButton{Qt::black, this}}
    , mLightPreview{new ColorPreviewButton{Qt::black, this}}
    , mMidLightPreview{new ColorPreviewButton{Qt::black, this}}
    , mDarkPreview{new ColorPreviewButton{Qt::black, this}}
    , mLinkPreview{new ColorPreviewButton{Qt::black, this}}
    , mLinkVisitedPreview{new ColorPreviewButton{Qt::black, this}}
    , mButtonPreview{new ColorPreviewButton{Qt::black, this}}
    , mButtonTextPreview{new ColorPreviewButton{Qt::black, this}}
    , mHighlightPreview{new ColorPreviewButton{Qt::black, this}}
    , mHighlightedTextPreview{new ColorPreviewButton{Qt::black, this}}
    , mPlaceholderTextPreview{new ColorPreviewButton{Qt::black, this}}
    , mTooltipBasePreview{new ColorPreviewButton{Qt::black, this}}
    , mTooltipTextPreview{new ColorPreviewButton{Qt::black, this}}
    , mTextPreview{new ColorPreviewButton{Qt::black, this}}
    , mShadowPreview{new ColorPreviewButton{Qt::black, this}}
{
  Q_ASSERT(layout);
  layout->addRow(tr("Base"), mBasePreview);
  layout->addRow(tr("Alternate Base"), mAlternateBasePreview);
  layout->addRow(tr("Window"), mWindowPreview);
  layout->addRow(tr("Window Text"), mWindowTextPreview);
  layout->addRow(tr("Light"), mLightPreview);
  layout->addRow(tr("Mid Light"), mMidLightPreview);
  layout->addRow(tr("Dark"), mDarkPreview);
  layout->addRow(tr("Link"), mLinkPreview);
  layout->addRow(tr("Link (visited)"), mLinkVisitedPreview);
  layout->addRow(tr("Button"), mButtonPreview);
  layout->addRow(tr("Button Text"), mButtonTextPreview);
  layout->addRow(tr("Highlight"), mHighlightPreview);
  layout->addRow(tr("Highlighted Text"), mHighlightedTextPreview);
  layout->addRow(tr("Placeholder Text"), mPlaceholderTextPreview);
  layout->addRow(tr("Tooltip Base"), mTooltipBasePreview);
  layout->addRow(tr("Tooltip Text"), mTooltipTextPreview);
  layout->addRow(tr("Text"), mTextPreview);
  layout->addRow(tr("Shadow"), mShadowPreview);

  const auto connectPreview = [this](ColorPreviewButton* button,
                                     const QPalette::ColorRole role) {
    connect(button,
            &ColorPreviewButton::S_ColorChanged,
            this,
            [this, role](const QColor& color) {
              emit S_ColorChanged(role, color);
            });
  };

  connectPreview(mBasePreview, QPalette::Base);
  connectPreview(mAlternateBasePreview, QPalette::AlternateBase);
  connectPreview(mWindowPreview, QPalette::Window);
  connectPreview(mWindowTextPreview, QPalette::WindowText);
  connectPreview(mLightPreview, QPalette::Light);
  connectPreview(mMidLightPreview, QPalette::Midlight);
  connectPreview(mDarkPreview, QPalette::Dark);
  connectPreview(mLinkPreview, QPalette::Link);
  connectPreview(mLinkVisitedPreview, QPalette::LinkVisited);
  connectPreview(mButtonPreview, QPalette::Button);
  connectPreview(mButtonTextPreview, QPalette::ButtonText);
  connectPreview(mHighlightPreview, QPalette::Highlight);
  connectPreview(mHighlightedTextPreview, QPalette::HighlightedText);
  connectPreview(mPlaceholderTextPreview, QPalette::PlaceholderText);
  connectPreview(mTooltipBasePreview, QPalette::ToolTipBase);
  connectPreview(mTooltipTextPreview, QPalette::ToolTipText);
  connectPreview(mTextPreview, QPalette::Text);
  connectPreview(mShadowPreview, QPalette::Shadow);
}

void ColorPreviewManager::UpdatePreview(const QPalette& palette)
{
  mBasePreview->SetColor(palette.color(mGroup, QPalette::Base));
  mAlternateBasePreview->SetColor(
      palette.color(mGroup, QPalette::AlternateBase));
  mWindowPreview->SetColor(palette.color(mGroup, QPalette::Window));
  mWindowTextPreview->SetColor(palette.color(mGroup, QPalette::WindowText));
  mLightPreview->SetColor(palette.color(mGroup, QPalette::Light));
  mMidLightPreview->SetColor(palette.color(mGroup, QPalette::Midlight));
  mDarkPreview->SetColor(palette.color(mGroup, QPalette::Dark));
  mLinkPreview->SetColor(palette.color(mGroup, QPalette::Link));
  mLinkVisitedPreview->SetColor(palette.color(mGroup, QPalette::LinkVisited));
  mButtonPreview->SetColor(palette.color(mGroup, QPalette::Button));
  mButtonTextPreview->SetColor(palette.color(mGroup, QPalette::ButtonText));
  mHighlightPreview->SetColor(palette.color(mGroup, QPalette::Highlight));
  mHighlightedTextPreview->SetColor(
      palette.color(mGroup, QPalette::HighlightedText));
  mPlaceholderTextPreview->SetColor(
      palette.color(mGroup, QPalette::PlaceholderText));
  mTooltipBasePreview->SetColor(palette.color(mGroup, QPalette::ToolTipBase));
  mTooltipTextPreview->SetColor(palette.color(mGroup, QPalette::ToolTipText));
  mTextPreview->SetColor(palette.color(mGroup, QPalette::Text));
  mShadowPreview->SetColor(palette.color(mGroup, QPalette::Shadow));
}

}  // namespace tactile
