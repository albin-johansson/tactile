#pragma once

#include <QUndoStack>
#include <utility>  // forward

namespace tactile {

class command_stack final : public QUndoStack {
 public:
  explicit command_stack(QObject* parent = nullptr);

  template <typename Command, typename... Args>
  void push(Args&&... args)
  {
    QUndoStack::push(new Command{std::forward<Args>(args)...});
  }
};

}  // namespace tactile
