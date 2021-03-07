#include "property_context_menu.hpp"

#include "icons.hpp"
#include "tactile_qstring.hpp"

namespace tactile::gui {

property_context_menu::property_context_menu(QWidget* parent)
    : QMenu{parent}
    , m_copy{addAction(icons::copy(), tr("Copy"))}
    , m_paste{addAction(icons::paste(), tr("Paste"))}
    , m_sep0{addSeparator()}
    , m_rename{addAction(icons::rename(), tr("Rename"))}
    , m_changeType{addMenu(icons::object_layer(), tr("Change type"))}
    , m_sep1{addSeparator()}
    , m_add{addAction(icons::add(), tr("Add new property..."))}
    , m_remove{addAction(icons::remove(), tr("Remove"))}
{
  m_copy->setShortcut(QKeySequence::Copy);
  m_paste->setShortcut(QKeySequence::Paste);
  m_rename->setShortcut(QKeySequence::fromString(TACTILE_QSTRING(u"CTRL+R")));
  m_remove->setShortcut(QKeySequence::Delete);

  m_copy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  m_paste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  m_rename->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  m_remove->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  m_typeString = m_changeType->addAction(tr("string"));
  m_typeInt = m_changeType->addAction(tr("int"));
  m_typeFloat = m_changeType->addAction(tr("float"));
  m_typeBool = m_changeType->addAction(tr("bool"));
  m_typeObject = m_changeType->addAction(tr("object"));
  m_typeColor = m_changeType->addAction(tr("color"));
  m_typeFile = m_changeType->addAction(tr("file"));

  connect(m_copy, &QAction::triggered, this, &property_context_menu::copy);
  connect(m_paste, &QAction::triggered, this, &property_context_menu::paste);
  connect(m_rename, &QAction::triggered, this, &property_context_menu::rename);
  connect(m_add, &QAction::triggered, this, &property_context_menu::add);
  connect(m_remove, &QAction::triggered, this, &property_context_menu::remove);

  connect(m_typeString, &QAction::triggered, [this] {
    emit change_type(core::property_type::string);
  });

  connect(m_typeInt, &QAction::triggered, [this] {
    emit change_type(core::property_type::integer);
  });

  connect(m_typeFloat, &QAction::triggered, [this] {
    emit change_type(core::property_type::floating);
  });

  connect(m_typeBool, &QAction::triggered, [this] {
    emit change_type(core::property_type::boolean);
  });

  connect(m_typeFile, &QAction::triggered, [this] {
    emit change_type(core::property_type::file);
  });

  connect(m_typeObject, &QAction::triggered, [this] {
    emit change_type(core::property_type::object);
  });

  connect(m_typeColor, &QAction::triggered, [this] {
    emit change_type(core::property_type::color);
  });

  add_actions(parent);
}

void property_context_menu::add_actions(QWidget* widget)
{
  Q_ASSERT(widget);
  widget->addAction(m_copy);
  widget->addAction(m_paste);
  widget->addAction(m_rename);
  widget->addAction(m_remove);
}

void property_context_menu::disable_all()
{
  m_copy->setEnabled(false);
  m_paste->setEnabled(false);
  m_rename->setEnabled(false);
  m_changeType->setEnabled(false);
  m_add->setEnabled(false);
  m_remove->setEnabled(false);
}

void property_context_menu::set_current_type(const core::property_type type)
{
  for (auto* elem : m_changeType->actions()) {
    if (!elem->isSeparator()) {
      elem->setEnabled(true);
    }
  }

  switch (type) {
    case core::property_type::string: {
      m_typeString->setEnabled(false);
      break;
    }
    case core::property_type::integer: {
      m_typeInt->setEnabled(false);
      break;
    }
    case core::property_type::floating: {
      m_typeFloat->setEnabled(false);
      break;
    }
    case core::property_type::boolean: {
      m_typeBool->setEnabled(false);
      break;
    }
    case core::property_type::file: {
      m_typeFile->setEnabled(false);
      break;
    }
    case core::property_type::color: {
      m_typeColor->setEnabled(false);
      break;
    }
    case core::property_type::object: {
      m_typeObject->setEnabled(false);
      break;
    }
  }
}

void property_context_menu::set_add_enabled(const bool enabled)
{
  m_add->setEnabled(enabled);
}

void property_context_menu::set_remove_enabled(const bool enabled)
{
  m_remove->setEnabled(enabled);
}

void property_context_menu::set_rename_enabled(const bool enabled)
{
  m_rename->setEnabled(enabled);
}

void property_context_menu::set_copy_enabled(const bool enabled)
{
  m_copy->setEnabled(enabled);
}

void property_context_menu::set_paste_enabled(const bool enabled)
{
  m_paste->setEnabled(enabled);
}

void property_context_menu::set_change_type_enabled(const bool enabled)
{
  m_changeType->setEnabled(enabled);
}

}  // namespace tactile::gui
