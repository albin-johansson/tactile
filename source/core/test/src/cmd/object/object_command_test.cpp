// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "test/object_command_test.hpp"

#include <utility>  // move

#include "tactile/core/cmd/object/create_object_command.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "test/document_testing.hpp"

namespace tactile::core {

void ObjectCommandTest::SetUp()
{
  Test::SetUp();

  {
    auto document = MapDocument::make(kOrthogonalMapSpec);
    ASSERT_TRUE(document.has_value());
    m_document = std::move(*document);
  }

  auto& registry = m_document->get_registry();
  m_map_id = registry.get<CDocumentInfo>().root;
  m_layer_id = make_object_layer(registry);
}

auto ObjectCommandTest::make_test_object() -> EntityID
{
  CreateObjectCommand create_object {&m_document.value(),
                                     m_layer_id,
                                     ObjectType::kRect,
                                     Float2 {12, 34},
                                     Float2 {56, 78}};
  create_object.redo();

  const auto& registry = m_document->get_registry();
  const auto& object_layer = registry.get<CObjectLayer>(m_layer_id);

  return object_layer.objects.back();
}

}  // namespace tactile::core
