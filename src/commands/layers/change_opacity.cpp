#include "change_opacity.hpp"

#include <QTranslator>

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::cmd {

change_opacity::change_opacity(not_null<core::map_document*> document,
                               const layer_id id,
                               const double opacity)
    : QUndoCommand{QTranslator::tr("Change Layer Opacity")}
    , m_document{document}
    , m_id{id}
    , m_opacity{opacity}
{
  if (!m_document)
  {
    throw tactile_error{"Null map document for change opacity command!"};
  }
}

void change_opacity::undo()
{
  QUndoCommand::undo();

  const auto opacity = m_previousOpacity.value();
  m_document->raw().set_opacity(m_id, opacity);
  emit m_document->changed_layer_opacity(m_id, opacity);
  emit m_document->redraw();

  m_previousOpacity.reset();
}

void change_opacity::redo()
{
  QUndoCommand::redo();

  auto& raw = m_document->raw();
  m_previousOpacity = raw.get_layer(m_id)->opacity();

  raw.set_opacity(m_id, m_opacity);
  emit m_document->changed_layer_opacity(m_id, m_opacity);
  emit m_document->redraw();
}

auto change_opacity::mergeWith(const QUndoCommand* other) -> bool
{
  if (id() == other->id())
  {
    if (auto* otherCommand = dynamic_cast<const change_opacity*>(other))
    {
      m_opacity = otherCommand->m_opacity;
      return true;
    }
  }

  return false;
}

}  // namespace tactile::cmd
