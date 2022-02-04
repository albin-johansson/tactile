#include "command.hpp"

#include <utility>  // move

namespace tactile {

command_base::command_base(std::string text) : mText{std::move(text)} {}

}  // namespace tactile
