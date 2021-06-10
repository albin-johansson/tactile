#pragma once

#include <QMenu>  // QMenu

#include "property.hpp"

namespace tactile {

class PropertyContextMenu final : public QMenu
{
  Q_OBJECT

 public:
  explicit PropertyContextMenu(QWidget* parent = nullptr);

  void DisableAll();

  void SetCurrentType(core::PropertyType type);

  void SetAddEnabled(bool enabled);

  void SetRemoveEnabled(bool enabled);

  void SetRenameEnabled(bool enabled);

  void SetCopyEnabled(bool enabled);

  void SetPasteEnabled(bool enabled);

  void SetChangeTypeEnabled(bool enabled);

 signals:
  void S_Add();
  void S_Remove();
  void S_Rename();
  void S_ChangeType(core::PropertyType type);
  void S_Copy();
  void S_Paste();

 private:
  QAction* mCopy{};
  QAction* mPaste{};
  [[maybe_unused]] QAction* mSep0;
  QAction* mRename{};
  QMenu* mChangeType{};
  [[maybe_unused]] QAction* mSep1;
  QAction* mAdd{};
  QAction* mRemove{};

  QAction* mTypeString{};
  QAction* mTypeInt{};
  QAction* mTypeFloat{};
  QAction* mTypeBool{};
  QAction* mTypeObject{};
  QAction* mTypeColor{};
  QAction* mTypeFile{};

  void AddActions(QWidget* widget);
};

}  // namespace tactile
