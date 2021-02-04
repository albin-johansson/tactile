#pragma once

#include <QMenu>  // QMenu

namespace tactile::gui {

class theme_options_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit theme_options_context_menu(QWidget* parent = nullptr);

 signals:
  void duplicate_theme();
  void import_theme();
  void export_theme();
  void reset_theme();

 private:
  QAction* m_duplicate{};
  [[maybe_unused]] QAction* m_sep0{};
  QAction* m_import{};
  QAction* m_export{};
  [[maybe_unused]] QAction* m_sep1{};
  QAction* m_reset{};
};

}  // namespace tactile::gui
