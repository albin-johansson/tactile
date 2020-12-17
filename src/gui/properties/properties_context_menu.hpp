#pragma once

#include <QMenu>

#include "property.hpp"

namespace tactile::gui {

class property_tree_item;

class properties_context_menu final : public QMenu
{
  Q_OBJECT

 public:
  explicit properties_context_menu(QWidget* parent = nullptr);

  void set_current_type(core::property::type type);

  void set_up_enabled(bool enabled);

  void set_down_enabled(bool enabled);

  void set_remove_enabled(bool enabled);

  void set_duplicate_enabled(bool enabled);

  void set_rename_enabled(bool enabled);

  void set_copy_enabled(bool enabled);

  void set_paste_enabled(bool enabled);

  void set_change_type_enabled(bool enabled);

 signals:
  void add();
  void remove();
  void rename();
  void move_up();
  void move_down();
  void duplicate();
  void change_type(core::property::type type);
  void copy();
  void paste();

 private:
  QAction* m_add{};
  [[maybe_unused]] QAction* m_sep0;
  QAction* m_rename{};
  QAction* m_duplicate{};
  [[maybe_unused]] QAction* m_sep1;
  QAction* m_copy{};
  QAction* m_paste{};
  [[maybe_unused]] QAction* m_sep2;
  QAction* m_moveUp{};
  QAction* m_moveDown{};
  [[maybe_unused]] QAction* m_sep3;
  QMenu* m_changeType{};
  [[maybe_unused]] QAction* m_sep4;
  QAction* m_remove{};

  QAction* m_typeString{};
  QAction* m_typeInt{};
  QAction* m_typeFloat{};
  QAction* m_typeBool{};
  QAction* m_typeObject{};
  QAction* m_typeColor{};
  QAction* m_typeFile{};
};

}  // namespace tactile::gui
