#include "xml_engine.hpp"

#include <iterator>  // make_move_iterator

#include "index_to_position.hpp"
#include "ints.hpp"
#include "tile_layer.hpp"
#include "to_property.hpp"

namespace tactile {

static_assert(IsParserObject<XmlElement>);
static_assert(IsParserEngine<XmlEngine, QDomDocument, XmlElement>);

auto XmlEngine::Tilesets(const object_type& root) -> std::vector<object_type>
{
  return Collect(root, QStringLiteral(u"tileset"));
}

auto XmlEngine::Layers(const object_type& root) -> std::vector<object_type>
{
  auto layers = Collect(root, QStringLiteral(u"layer"));
  auto objectLayers = Collect(root, QStringLiteral(u"objectgroup"));

  // Move all object layers into the vector with tile layers
  layers.insert(layers.end(),
                std::make_move_iterator(objectLayers.begin()),
                std::make_move_iterator(objectLayers.end()));

  return layers;
}

auto XmlEngine::Properties(const object_type& object)
    -> std::vector<object_type>
{
  const auto elems = object->elementsByTagName(QStringLiteral(u"properties"));

  if (elems.isEmpty())
  {
    return {};
  }

  const auto size = elems.size();
  for (int i = 0; i < size; ++i)
  {
    const auto node = elems.at(i);
    Q_ASSERT(node.isElement());

    const auto parent = node.parentNode();
    Q_ASSERT(parent.isElement());

    const auto parentElement = parent.toElement();
    if (parentElement.tagName() == object->tagName())
    {
      const auto element = node.toElement();
      return Collect(object_type{element}, QStringLiteral(u"property"));
    }
  }

  return {};
}

auto XmlEngine::Objects(const object_type& object) -> std::vector<object_type>
{
  return Collect(object, QStringLiteral(u"object"));
}

auto XmlEngine::Tiles(const object_type& object,
                      const row_t nRows,
                      const col_t nCols,
                      ParseError& error) -> core::tile_matrix
{
  auto matrix = core::MakeTileMatrix(nRows, nCols);

  const auto data = object->firstChildElement(QStringLiteral(u"data"));
  const auto tiles = data.text().split(u',');

  int index{0};
  for (const auto& value : tiles)
  {
    bool ok{};
    const tile_id id{value.toInt(&ok)};

    if (!ok)
    {
      error = ParseError::LayerCouldNotParseTile;
      return matrix;
    }

    const auto pos = IndexToPosition(index, nCols);
    matrix.at(pos.RowIndex()).at(pos.ColumnIndex()) = id;

    ++index;
  }

  return matrix;
}

auto XmlEngine::PropertyType(const object_type& object) -> QString
{
  /* The following is a quirk due to the fact that the type attribute can be
     omitted for string properties */
  if (AssumeStringProperty(object))
  {
    return QStringLiteral(u"string");
  }
  else
  {
    return object.String(ElementId::Type).value();
  }
}

auto XmlEngine::Root(const document_type& document) -> object_type
{
  return object_type{document.documentElement()};
}

auto XmlEngine::FromFile(const QFileInfo& path) -> Maybe<document_type>
{
  return ReadXml(path);
}

auto XmlEngine::TilesetImageRelativePath(const object_type& object)
    -> Maybe<QString>
{
  const auto imageElem = object->firstChildElement(QStringLiteral(u"image"));
  const auto path = imageElem.attribute(QStringLiteral(u"source"));
  if (!path.isNull())
  {
    return path;
  }
  else
  {
    return nothing;
  }
}

auto XmlEngine::ContainsTilesets(const object_type& object) -> bool
{
  return true;
}

auto XmlEngine::ValidateLayerType(const object_type& object) -> bool
{
  return true;
}

auto XmlEngine::ContainsLayers(const object_type& object) -> bool
{
  return true;
}

auto XmlEngine::AssumeStringProperty(const object_type& object) -> bool
{
  return !object->hasAttribute(QStringLiteral(u"type"));
}

auto XmlEngine::IsTileLayer(const object_type& object) -> bool
{
  return object->tagName() == QStringLiteral(u"layer");
}

auto XmlEngine::IsObjectLayer(const object_type& object) -> bool
{
  return object->tagName() == QStringLiteral(u"objectgroup");
}

auto XmlEngine::IsPoint(const object_type& object) -> bool
{
  return HasChild(object, QStringLiteral(u"point"));
}

auto XmlEngine::Collect(const object_type& root, const QString& key)
    -> std::vector<object_type>
{
  const auto elements = root->elementsByTagName(key);
  const auto count = elements.count();

  std::vector<object_type> result;
  result.reserve(static_cast<usize>(count));

  for (auto i = 0; i < count; ++i)
  {
    const auto& node = elements.at(i);

    Q_ASSERT(node.isElement());
    result.emplace_back(node.toElement());
  }

  return result;
}

auto XmlEngine::HasChild(const object_type& obj, const QString& tag) -> bool
{
  return !obj->firstChildElement(tag).isNull();
}

}  // namespace tactile
