#pragma once

#include <QUndoStack>
#include <utility>

namespace tactile {

/**
 * @class command_stack
 *
 * @brief Represents a stack of commands.
 *
 * @since 0.1.0
 *
 * @headerfile command_stack.hpp
 */
class command_stack final : public QUndoStack {
 public:
  explicit command_stack(QObject* parent = nullptr);

  /**
   * @brief Pushes a command onto the undo stack after executing it.
   *
   * @tparam Command the type of the command.
   * @tparam Args the types of the arguments that will be forwarded.
   *
   * @param args the arguments that will be forwarded to the command
   * constructor.
   *
   * @since 0.1.0
   */
  template <typename Command, typename... Args>
  void push(Args&&... args)
  {
    QUndoStack::push(new Command{std::forward<Args>(args)...});
  }
};

}  // namespace tactile
