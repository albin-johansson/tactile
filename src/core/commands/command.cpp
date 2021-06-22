#include "command.hpp"

#include <utility>  // move

namespace tactile {

ACommand::ACommand(std::string text) : mText{std::move(text)}
{}

}  // namespace tactile
