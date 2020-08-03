#include "command_stack.hpp"

namespace tactile {

command_stack::command_stack(QObject* parent) : QUndoStack{parent}
{}

}  // namespace tactile
