#include "map_item.hpp"

#include <QStyleOption>  // QStyleOption

#include "map.hpp"
#include "map_document.hpp"
#include "position.hpp"
#include "render_map.hpp"
#include "tactile_error.hpp"
#include "tile_layer.hpp"

namespace tactile::gui {

map_item::map_item(core::map_document* map, QGraphicsItem* parent)
    : QGraphicsItem{parent}
    , m_document{map}
{
  if (!m_document) {
    throw tactile_error{"Cannot create map item based on null map document!"};
  }
  // This is to be able to use exposedRect
  setFlags(QGraphicsItem::ItemUsesExtendedStyleOption);
}

void map_item::disable_stamp_preview()
{
  m_mousePosition.reset();
  update();
}

void map_item::enable_stamp_preview(const core::position& position)
{
  m_mousePosition = position;
}

void map_item::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem* option,
                     QWidget*)
{
  Q_ASSERT(painter);
  Q_ASSERT(option);
  Q_ASSERT(m_document);
  render_map(*painter, *m_document, m_mousePosition, option->exposedRect);
}

auto map_item::boundingRect() const -> QRectF
{
  const auto width = static_cast<double>(m_document->width());
  const auto height = static_cast<double>(m_document->height());
  return {0, 0, width, height};
}

}  // namespace tactile::gui
