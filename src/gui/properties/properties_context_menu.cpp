#include "properties_context_menu.hpp"

#include "icons.hpp"

namespace tactile::gui {

properties_context_menu::properties_context_menu(QWidget* parent)
    : QMenu{parent}
    , m_add{addAction(icons::add(), tr("Add property..."))}
    , m_sep0{addSeparator()}
    , m_rename{addAction(icons::rename(), tr("Rename"))}
    , m_sep1{addSeparator()}
    , m_copy{addAction(icons::copy(), tr("Copy"))}
    , m_paste{addAction(icons::paste(), tr("Paste"))}
    , m_duplicate{addAction(icons::duplicate(), tr("Duplicate"))}
    , m_sep2{addSeparator()}
    , m_moveUp{addAction(icons::move_up(), tr("Move up"))}
    , m_moveDown{addAction(icons::move_down(), tr("Move down"))}
    , m_sep3{addSeparator()}
    , m_changeType{addMenu(icons::object(), tr("Change type"))}
    , m_sep4{addSeparator()}
    , m_remove{addAction(icons::remove(), tr("Remove"))}
{
  m_typeString = m_changeType->addAction(tr("string"));
  m_typeInt = m_changeType->addAction(tr("int"));
  m_typeFloat = m_changeType->addAction(tr("float"));
  m_typeBool = m_changeType->addAction(tr("bool"));
  m_typeObject = m_changeType->addAction(tr("object"));
  m_typeColor = m_changeType->addAction(tr("color"));
  m_typeFile = m_changeType->addAction(tr("file"));

  using context_menu = properties_context_menu;
  connect(m_add, &QAction::triggered, this, &context_menu::add);
  connect(m_remove, &QAction::triggered, this, &context_menu::remove);
  connect(m_rename, &QAction::triggered, this, &context_menu::rename);
  connect(m_moveUp, &QAction::triggered, this, &context_menu::move_up);
  connect(m_moveDown, &QAction::triggered, this, &context_menu::move_down);
  connect(m_duplicate, &QAction::triggered, this, &context_menu::duplicate);
  connect(m_copy, &QAction::triggered, this, &context_menu::copy);
  connect(m_paste, &QAction::triggered, this, &context_menu::paste);

  connect(m_typeString, &QAction::triggered, [this] {
    emit change_type(core::property::string);
  });

  connect(m_typeInt, &QAction::triggered, [this] {
    emit change_type(core::property::integer);
  });

  connect(m_typeFloat, &QAction::triggered, [this] {
    emit change_type(core::property::floating);
  });

  connect(m_typeBool, &QAction::triggered, [this] {
    emit change_type(core::property::boolean);
  });

  connect(m_typeFile, &QAction::triggered, [this] {
    emit change_type(core::property::file);
  });

  connect(m_typeObject, &QAction::triggered, [this] {
    emit change_type(core::property::object);
  });

  connect(m_typeColor, &QAction::triggered, [this] {
    emit change_type(core::property::color);
  });
}

void properties_context_menu::set_current_type(const core::property::type type)
{
  for (auto* elem : m_changeType->actions()) {
    if (!elem->isSeparator()) {
      elem->setEnabled(true);
    }
  }

  switch (type) {
    case core::property::string: {
      m_typeString->setEnabled(false);
      break;
    }
    case core::property::integer: {
      m_typeInt->setEnabled(false);
      break;
    }
    case core::property::floating: {
      m_typeFloat->setEnabled(false);
      break;
    }
    case core::property::boolean: {
      m_typeBool->setEnabled(false);
      break;
    }
    case core::property::file: {
      m_typeFile->setEnabled(false);
      break;
    }
    case core::property::color: {
      m_typeColor->setEnabled(false);
      break;
    }
    case core::property::object: {
      m_typeObject->setEnabled(false);
      break;
    }
  }
}

void properties_context_menu::set_up_enabled(const bool enabled)
{
  m_moveUp->setEnabled(enabled);
}

void properties_context_menu::set_down_enabled(const bool enabled)
{
  m_moveDown->setEnabled(enabled);
}

void properties_context_menu::set_remove_enabled(const bool enabled)
{
  m_remove->setEnabled(enabled);
}

void properties_context_menu::set_duplicate_enabled(const bool enabled)
{
  m_duplicate->setEnabled(enabled);
}

void properties_context_menu::set_rename_enabled(const bool enabled)
{
  m_rename->setEnabled(enabled);
}

void properties_context_menu::set_copy_enabled(const bool enabled)
{
  m_copy->setEnabled(enabled);
}

void properties_context_menu::set_paste_enabled(const bool enabled)
{
  m_paste->setEnabled(enabled);
}

void properties_context_menu::set_change_type_enabled(const bool enabled)
{
  m_changeType->setEnabled(enabled);
}

}  // namespace tactile::gui
