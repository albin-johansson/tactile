#pragma once

#include <QUndoCommand>
#include <memory>  // shared_ptr

#include "map_document.hpp"
#include "tileset.hpp"

namespace tactile::cmd {

class add_tileset final : public QUndoCommand
{
 public:
  add_tileset(core::map_document* document,
              std::shared_ptr<core::tileset> tileset,
              tileset_id id);

  void undo() override;

  void redo() override;

 private:
  core::map_document* m_document;
  std::shared_ptr<core::tileset> m_tileset;
  tileset_id m_id;
};

}  // namespace tactile::cmd
