#pragma once

#include <QMenu>

namespace tactile::gui {

class layer_item_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit layer_item_context_menu(QWidget* parent = nullptr);

  void set_visibility_enabled(bool enabled);

  void set_remove_enabled(bool enabled);

  void set_move_up_enabled(bool enabled);

  void set_move_down_enabled(bool enabled);

 signals:
  void toggle_visibility();
  void move_layer_up();
  void move_layer_down();
  void remove_layer();

 private:
  QAction m_visibility;
  QAction m_moveUp;
  QAction m_sep0;
  QAction m_moveDown;
  QAction m_sep1;
  QAction m_remove;

  [[nodiscard]] static auto visibility_icon() -> const QIcon&;
  [[nodiscard]] static auto move_up_icon() -> const QIcon&;
  [[nodiscard]] static auto move_down_icon() -> const QIcon&;
  [[nodiscard]] static auto remove_icon() -> const QIcon&;
};

}  // namespace tactile::gui
