#include "component.hpp"

#include <utility>  // move

namespace Tactile {

Component::Component(std::string type) : mType{std::move(type)}
{}

}  // namespace Tactile