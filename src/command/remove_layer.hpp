#pragma once

#include <QUndoCommand>
#include <memory>  // shared_ptr

#include "layer.hpp"
#include "layer_id.hpp"

namespace tactile {
namespace core {
class map_document;
}

namespace cmd {

class remove_layer final : public QUndoCommand
{
 public:
  remove_layer(core::map_document* document, layer_id id);

  void undo() override;

  void redo() override;

 private:
  core::map_document* m_document{};
  std::shared_ptr<core::layer> m_layer;
  layer_id m_id;
  int m_index{};
};

}  // namespace cmd
}  // namespace tactile
