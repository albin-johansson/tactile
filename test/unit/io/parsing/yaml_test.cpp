#include <gtest/gtest.h>

// clang-format off
#include <ryml_std.hpp>
#include <ryml.hpp>
// clang-format on

#include <fstream>    // ifstream
#include <istream>    // istream
#include <stdexcept>  // runtime_error
#include <string>     // string

[[nodiscard]] static auto ToString(std::istream& stream) -> std::string
{
  std::string result;
  result.reserve(128);

  char buffer[4096];
  while (stream.read(buffer, sizeof(buffer)))
  {
    result.append(buffer, sizeof(buffer));
  }
  result.append(buffer, stream.gcount());

  return result;
}

TEST(YAML, Usage)
{
  ryml::set_error_callback(
      [](const char* msg, size_t size) { throw std::runtime_error{msg}; });

  std::ifstream stream{"../src/resources/proto.yaml"};
  auto str = ToString(stream);
  auto tree = ryml::parse(ryml::substr(str.data(), str.size()));

  const auto root = tree.rootref();
  ASSERT_TRUE(root.valid());
  ASSERT_EQ(ryml::NodeType_e::STREAM, root.type());
  ASSERT_EQ(1, root.num_children());

  const auto doc = root.first_child();
  ASSERT_EQ(ryml::NodeType_e::DOCMAP, doc.type());

  ASSERT_EQ("Tactile map", doc["name"].val());
  ASSERT_EQ("1", doc["version"].val());
  ASSERT_EQ("4", doc["row-count"].val());
  ASSERT_EQ("5", doc["column-count"].val());
}
