#include "command.hpp"

#include <utility>  // move

namespace Tactile {

ACommand::ACommand(std::string text) : mText{std::move(text)} {}

}  // namespace Tactile
