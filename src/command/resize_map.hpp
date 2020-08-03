#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

class resize_map final : public QUndoCommand {
 public:
  explicit resize_map(model::core* core, int nRows, int nCols);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
  int m_rows;
  int m_cols;
  int m_oldRows{1};
  int m_oldCols{1};
};

}  // namespace tactile::cmd
