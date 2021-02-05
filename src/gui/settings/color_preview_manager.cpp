#include "color_preview_manager.hpp"

#include "color_preview_button.hpp"

namespace tactile::gui {

color_preview_manager::color_preview_manager(QFormLayout* layout)
    : QWidget{}
    , m_basePreview{new color_preview_button{Qt::black, this}}
    , m_alternateBasePreview{new color_preview_button{Qt::black, this}}
    , m_windowPreview{new color_preview_button{Qt::black, this}}
    , m_windowTextPreview{new color_preview_button{Qt::black, this}}
    , m_lightPreview{new color_preview_button{Qt::black, this}}
    , m_midLightPreview{new color_preview_button{Qt::black, this}}
    , m_darkPreview{new color_preview_button{Qt::black, this}}
    , m_linkPreview{new color_preview_button{Qt::black, this}}
    , m_linkVisitedPreview{new color_preview_button{Qt::black, this}}
    , m_buttonPreview{new color_preview_button{Qt::black, this}}
    , m_buttonTextPreview{new color_preview_button{Qt::black, this}}
    , m_highlightPreview{new color_preview_button{Qt::black, this}}
    , m_highlightedTextPreview{new color_preview_button{Qt::black, this}}
    , m_placeholderTextPreview{new color_preview_button{Qt::black, this}}
    , m_tooltipBasePreview{new color_preview_button{Qt::black, this}}
    , m_tooltipTextPreview{new color_preview_button{Qt::black, this}}
    , m_textPreview{new color_preview_button{Qt::black, this}}
    , m_shadowPreview{new color_preview_button{Qt::black, this}}
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
}

void color_preview_manager::update_preview(const QPalette& palette)
{
  m_basePreview->set_color(palette.base().color());
  m_alternateBasePreview->set_color(palette.alternateBase().color());
  m_windowPreview->set_color(palette.window().color());
  m_windowTextPreview->set_color(palette.windowText().color());
  m_lightPreview->set_color(palette.light().color());
  m_midLightPreview->set_color(palette.midlight().color());
  m_darkPreview->set_color(palette.dark().color());
  m_linkPreview->set_color(palette.link().color());
  m_linkVisitedPreview->set_color(palette.linkVisited().color());
  m_buttonPreview->set_color(palette.button().color());
  m_buttonTextPreview->set_color(palette.buttonText().color());
  m_highlightPreview->set_color(palette.highlight().color());
  m_highlightedTextPreview->set_color(palette.highlightedText().color());
  m_placeholderTextPreview->set_color(palette.placeholderText().color());
  m_tooltipBasePreview->set_color(palette.toolTipBase().color());
  m_tooltipTextPreview->set_color(palette.toolTipText().color());
  m_textPreview->set_color(palette.text().color());
  m_shadowPreview->set_color(palette.shadow().color());
}

}  // namespace tactile::gui
