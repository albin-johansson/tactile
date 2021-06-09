#include "color_preview_manager.hpp"

#include "color_preview_button.hpp"

namespace tactile::gui {

color_preview_manager::color_preview_manager(QFormLayout* layout,
                                             const QPalette::ColorGroup group,
                                             QWidget* parent)
    : QWidget{parent}
    , m_group{group}
    , m_basePreview{new ColorPreviewButton{Qt::black, this}}
    , m_alternateBasePreview{new ColorPreviewButton{Qt::black, this}}
    , m_windowPreview{new ColorPreviewButton{Qt::black, this}}
    , m_windowTextPreview{new ColorPreviewButton{Qt::black, this}}
    , m_lightPreview{new ColorPreviewButton{Qt::black, this}}
    , m_midLightPreview{new ColorPreviewButton{Qt::black, this}}
    , m_darkPreview{new ColorPreviewButton{Qt::black, this}}
    , m_linkPreview{new ColorPreviewButton{Qt::black, this}}
    , m_linkVisitedPreview{new ColorPreviewButton{Qt::black, this}}
    , m_buttonPreview{new ColorPreviewButton{Qt::black, this}}
    , m_buttonTextPreview{new ColorPreviewButton{Qt::black, this}}
    , m_highlightPreview{new ColorPreviewButton{Qt::black, this}}
    , m_highlightedTextPreview{new ColorPreviewButton{Qt::black, this}}
    , m_placeholderTextPreview{new ColorPreviewButton{Qt::black, this}}
    , m_tooltipBasePreview{new ColorPreviewButton{Qt::black, this}}
    , m_tooltipTextPreview{new ColorPreviewButton{Qt::black, this}}
    , m_textPreview{new ColorPreviewButton{Qt::black, this}}
    , m_shadowPreview{new ColorPreviewButton{Qt::black, this}}
{
  Q_ASSERT(layout);
  layout->addRow(tr("Base"), m_basePreview);
  layout->addRow(tr("Alternate Base"), m_alternateBasePreview);
  layout->addRow(tr("Window"), m_windowPreview);
  layout->addRow(tr("Window Text"), m_windowTextPreview);
  layout->addRow(tr("Light"), m_lightPreview);
  layout->addRow(tr("Mid Light"), m_midLightPreview);
  layout->addRow(tr("Dark"), m_darkPreview);
  layout->addRow(tr("Link"), m_linkPreview);
  layout->addRow(tr("Link (visited)"), m_linkVisitedPreview);
  layout->addRow(tr("Button"), m_buttonPreview);
  layout->addRow(tr("Button Text"), m_buttonTextPreview);
  layout->addRow(tr("Highlight"), m_highlightPreview);
  layout->addRow(tr("Highlighted Text"), m_highlightedTextPreview);
  layout->addRow(tr("Placeholder Text"), m_placeholderTextPreview);
  layout->addRow(tr("Tooltip Base"), m_tooltipBasePreview);
  layout->addRow(tr("Tooltip Text"), m_tooltipTextPreview);
  layout->addRow(tr("Text"), m_textPreview);
  layout->addRow(tr("Shadow"), m_shadowPreview);

  const auto connectPreview = [this](ColorPreviewButton* button,
                                     const QPalette::ColorRole role) {
    connect(button,
            &ColorPreviewButton::S_ColorChanged,
            this,
            [this, role](const QColor& color) {
              emit color_changed(role, color);
            });
  };

  connectPreview(m_basePreview, QPalette::Base);
  connectPreview(m_alternateBasePreview, QPalette::AlternateBase);
  connectPreview(m_windowPreview, QPalette::Window);
  connectPreview(m_windowTextPreview, QPalette::WindowText);
  connectPreview(m_lightPreview, QPalette::Light);
  connectPreview(m_midLightPreview, QPalette::Midlight);
  connectPreview(m_darkPreview, QPalette::Dark);
  connectPreview(m_linkPreview, QPalette::Link);
  connectPreview(m_linkVisitedPreview, QPalette::LinkVisited);
  connectPreview(m_buttonPreview, QPalette::Button);
  connectPreview(m_buttonTextPreview, QPalette::ButtonText);
  connectPreview(m_highlightPreview, QPalette::Highlight);
  connectPreview(m_highlightedTextPreview, QPalette::HighlightedText);
  connectPreview(m_placeholderTextPreview, QPalette::PlaceholderText);
  connectPreview(m_tooltipBasePreview, QPalette::ToolTipBase);
  connectPreview(m_tooltipTextPreview, QPalette::ToolTipText);
  connectPreview(m_textPreview, QPalette::Text);
  connectPreview(m_shadowPreview, QPalette::Shadow);
}

void color_preview_manager::update_preview(const QPalette& palette)
{
  m_basePreview->SetColor(palette.color(m_group, QPalette::Base));
  m_alternateBasePreview->SetColor(
      palette.color(m_group, QPalette::AlternateBase));
  m_windowPreview->SetColor(palette.color(m_group, QPalette::Window));
  m_windowTextPreview->SetColor(palette.color(m_group, QPalette::WindowText));
  m_lightPreview->SetColor(palette.color(m_group, QPalette::Light));
  m_midLightPreview->SetColor(palette.color(m_group, QPalette::Midlight));
  m_darkPreview->SetColor(palette.color(m_group, QPalette::Dark));
  m_linkPreview->SetColor(palette.color(m_group, QPalette::Link));
  m_linkVisitedPreview->SetColor(palette.color(m_group, QPalette::LinkVisited));
  m_buttonPreview->SetColor(palette.color(m_group, QPalette::Button));
  m_buttonTextPreview->SetColor(palette.color(m_group, QPalette::ButtonText));
  m_highlightPreview->SetColor(palette.color(m_group, QPalette::Highlight));
  m_highlightedTextPreview->SetColor(
      palette.color(m_group, QPalette::HighlightedText));
  m_placeholderTextPreview->SetColor(
      palette.color(m_group, QPalette::PlaceholderText));
  m_tooltipBasePreview->SetColor(palette.color(m_group, QPalette::ToolTipBase));
  m_tooltipTextPreview->SetColor(palette.color(m_group, QPalette::ToolTipText));
  m_textPreview->SetColor(palette.color(m_group, QPalette::Text));
  m_shadowPreview->SetColor(palette.color(m_group, QPalette::Shadow));
}

}  // namespace tactile::gui
