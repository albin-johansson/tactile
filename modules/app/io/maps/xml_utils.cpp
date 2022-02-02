#include "xml_utils.hpp"

#include <algorithm>  // any_of
#include <cstring>    // strcmp
#include <limits>     // numeric_limits

#include "assert.hpp"

namespace tactile {
namespace {

constexpr auto _error_int = std::numeric_limits<int32>::max();
constexpr auto _error_uint = std::numeric_limits<uint32>::max();
constexpr auto _error_float = std::numeric_limits<float>::max();

}  // namespace

auto collect_children(pugi::xml_node source, const char* name)
    -> std::vector<pugi::xml_node>
{
  std::vector<pugi::xml_node> children;

  for (auto child : source.children(name)) {
    children.push_back(child);
  }

  return children;
}

auto has_attribute(pugi::xml_node node, const char* attributeName) -> bool
{
  TACTILE_ASSERT(attributeName != nullptr);

  const auto begin = node.attributes_begin();
  const auto end = node.attributes_end();

  return std::any_of(begin, end, [attributeName](const pugi::xml_attribute attribute) {
    return std::strcmp(attribute.name(), attributeName) == 0;
  });
}

auto string_attribute(pugi::xml_node node, const char* attributeName)
    -> maybe<std::string>
{
  TACTILE_ASSERT(attributeName);

  if (const char* str = node.attribute(attributeName).as_string(nullptr)) {
    return str;
  }
  else {
    return nothing;
  }
}

auto int_attribute(pugi::xml_node node, const char* attributeName) -> maybe<int32>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  const auto value = attribute.as_int(_error_int);

  if (!attribute.empty() && value != _error_int) {
    return value;
  }
  else {
    return nothing;
  }
}

auto uint_attribute(pugi::xml_node node, const char* attributeName) -> maybe<uint32>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  const auto value = attribute.as_uint(_error_uint);

  if (!attribute.empty() && value != _error_uint) {
    return value;
  }
  else {
    return nothing;
  }
}

auto float_attribute(pugi::xml_node node, const char* attributeName) -> maybe<float>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  const auto value = attribute.as_float(_error_float);

  if (!attribute.empty() && value != _error_float) {
    return value;
  }
  else {
    return nothing;
  }
}

auto bool_attribute(pugi::xml_node node, const char* attributeName) -> maybe<bool>
{
  TACTILE_ASSERT(attributeName);

  const auto attribute = node.attribute(attributeName);
  if (!attribute.empty()) {
    return attribute.as_bool();
  }
  else {
    return nothing;
  }
}

}  // namespace tactile