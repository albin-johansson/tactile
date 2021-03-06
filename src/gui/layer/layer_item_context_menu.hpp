#pragma once

#include <QMenu>  // QMenu

namespace tactile::gui {

class layer_item_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit layer_item_context_menu(QWidget* parent);

  void set_visibility_enabled(bool enabled);

  void set_remove_enabled(bool enabled);

  void set_move_up_enabled(bool enabled);

  void set_move_down_enabled(bool enabled);

 signals:
  void toggle_visibility();
  void show_properties();
  void move_layer_up();
  void move_layer_down();
  void duplicate_layer();
  void remove_layer();

 private:
  QAction* m_visibility{};
  QAction* m_showProperties{};
  [[maybe_unused]] QAction* m_sep0{};
  QAction* m_moveUp{};
  QAction* m_moveDown{};
  [[maybe_unused]] QAction* m_sep1{};
  QAction* m_duplicate{};
  QAction* m_remove{};
};

}  // namespace tactile::gui
