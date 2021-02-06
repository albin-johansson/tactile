#include "theme_options_context_menu.hpp"

#include "icons.hpp"

namespace tactile::gui {

theme_options_context_menu::theme_options_context_menu(QWidget* parent)
    : QMenu{parent}
    , m_rename{addAction(icons::rename(), tr("Rename..."))}
    , m_duplicate{addAction(icons::duplicate(), tr("Duplicate..."))}
    , m_sep0{addSeparator()}
    , m_import{addAction(icons::open(), tr("Import theme..."))}
    , m_export{addAction(icons::save_as(), tr("Export theme..."))}
    , m_sep1{addSeparator()}
    , m_reset{addAction(icons::reset(), tr("Reset"))}
    , m_sep2{addSeparator()}
    , m_remove{addAction(icons::remove(), tr("Remove"))}
{
  // clang-format off
  connect(m_rename, &QAction::triggered,
          this, &theme_options_context_menu::rename_theme);

  connect(m_duplicate, &QAction::triggered,
          this, &theme_options_context_menu::duplicate_theme);

  connect(m_import, &QAction::triggered,
          this, &theme_options_context_menu::import_theme);

  connect(m_export, &QAction::triggered,
          this, &theme_options_context_menu::export_theme);

  connect(m_reset, &QAction::triggered,
          this, &theme_options_context_menu::reset_theme);

  connect(m_remove, &QAction::triggered,
          this, &theme_options_context_menu::remove_theme);
  // clang-format on
}

void theme_options_context_menu::set_rename_enabled(const bool enabled)
{
  m_rename->setEnabled(enabled);
}

void theme_options_context_menu::set_reset_enabled(const bool enabled)
{
  m_reset->setEnabled(enabled);
}

void theme_options_context_menu::set_remove_enabled(const bool enabled)
{
  m_remove->setEnabled(enabled);
}

}  // namespace tactile::gui
