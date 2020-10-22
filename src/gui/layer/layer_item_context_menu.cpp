#include "layer_item_context_menu.hpp"

namespace tactile::gui {

layer_item_context_menu::layer_item_context_menu(QWidget* parent)
    : QMenu{parent},
      m_visibility{tr("Toggle visibility"), this},
      m_moveUp{tr("Move layer up"), this},
      m_moveDown{tr("Move layer down"), this},
      m_duplicate{tr("Duplicate layer"), this},
      m_remove{tr("Remove layer"), this}
{
  m_visibility.setIcon(visibility_icon());
  m_moveUp.setIcon(move_up_icon());
  m_moveDown.setIcon(move_down_icon());
  m_duplicate.setIcon(duplicate_icon());
  m_remove.setIcon(remove_icon());

  // clang-format off
  connect(&m_visibility, &QAction::triggered, this, &layer_item_context_menu::toggle_visibility);
  connect(&m_moveUp, &QAction::triggered, this, &layer_item_context_menu::move_layer_up);
  connect(&m_moveDown, &QAction::triggered, this, &layer_item_context_menu::move_layer_down);
  connect(&m_duplicate, &QAction::triggered, this, &layer_item_context_menu::duplicate_layer);
  connect(&m_remove, &QAction::triggered, this, &layer_item_context_menu::remove_layer);
  // clang-format on

  m_sep0.setSeparator(true);
  m_sep1.setSeparator(true);

  addAction(&m_visibility);
  addAction(&m_sep0);
  addAction(&m_moveUp);
  addAction(&m_moveDown);
  addAction(&m_sep1);
  addAction(&m_duplicate);
  addAction(&m_remove);
}

void layer_item_context_menu::set_visibility_enabled(bool enabled)
{
  m_visibility.setEnabled(enabled);
}

void layer_item_context_menu::set_remove_enabled(bool enabled)
{
  m_remove.setEnabled(enabled);
}

void layer_item_context_menu::set_move_up_enabled(bool enabled)
{
  m_moveUp.setEnabled(enabled);
}

void layer_item_context_menu::set_move_down_enabled(bool enabled)
{
  m_moveDown.setEnabled(enabled);
}

auto layer_item_context_menu::visibility_icon() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/visible.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto layer_item_context_menu::move_up_icon() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/up.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto layer_item_context_menu::move_down_icon() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/down.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto layer_item_context_menu::duplicate_icon() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/new-copy.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

auto layer_item_context_menu::remove_icon() -> const QIcon&
{
  static const QIcon icon{
      QStringLiteral(u":resources/icons/icons8/color/64/remove.png")};
  Q_ASSERT(!icon.isNull());
  return icon;
}

}  // namespace tactile::gui
