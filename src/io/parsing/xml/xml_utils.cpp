#include "xml_utils.hpp"

#include <algorithm>  // any_of
#include <cassert>    // assert
#include <cstring>    // strcmp
#include <limits>     // numeric_limits
#include <utility>    // move

namespace Tactile::IO {
namespace {

constexpr int error_int = std::numeric_limits<int>::lowest();
constexpr float error_float = std::numeric_limits<float>::lowest();

}  // namespace

auto Collect(const pugi::xml_node node, const czstring str)
    -> std::vector<pugi::xml_node>
{
  std::vector<pugi::xml_node> nodes;

  for (auto child : node.children(str))
  {
    nodes.push_back(std::move(child));
  }

  return nodes;
}

auto Contains(const pugi::xml_node node, const NotNull<czstring> attr) -> bool
{
  assert(attr);

  const auto begin = node.attributes_begin();
  const auto end = node.attributes_end();

  return std::any_of(begin, end, [attr](const pugi::xml_attribute attribute) {
    return std::strcmp(attribute.name(), attr) == 0;
  });
}

auto GetString(const pugi::xml_node node, const NotNull<czstring> attr)
    -> Maybe<std::string>
{
  assert(attr);

  if (const auto str = node.attribute(attr).as_string(nullptr))
  {
    return str;
  }
  else
  {
    return nothing;
  }
}

auto GetInt(const pugi::xml_node node, const NotNull<czstring> attr) -> Maybe<int>
{
  assert(attr);

  const auto attribute = node.attribute(attr);
  const auto value = attribute.as_int(error_int);

  if (!attribute.empty() && value != error_int)
  {
    return value;
  }
  else
  {
    return nothing;
  }
}

auto GetFloat(const pugi::xml_node node, const NotNull<czstring> attr)
    -> Maybe<float>
{
  assert(attr);

  const auto attribute = node.attribute(attr);
  const auto value = attribute.as_float(error_float);

  if (!attribute.empty() && value != error_float)
  {
    return value;
  }
  else
  {
    return nothing;
  }
}

auto GetBool(const pugi::xml_node node, const NotNull<czstring> attr) -> Maybe<bool>
{
  assert(attr);

  const auto attribute = node.attribute(attr);
  if (!attribute.empty())
  {
    return attribute.as_bool();
  }
  else
  {
    return nothing;
  }
}

}  // namespace Tactile::IO
