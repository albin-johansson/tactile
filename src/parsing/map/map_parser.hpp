#pragma once

#include <QDir>       // QDir
#include <QFileInfo>  // QFileInfo
#include <concepts>   // same_as
#include <ranges>     // all_of
#include <vector>     // vector

#include "element_id.hpp"
#include "ints.hpp"
#include "map_document.hpp"
#include "map_parse_concepts.hpp"
#include "map_parse_data.hpp"
#include "maybe.hpp"
#include "object_id.hpp"
#include "parse_error.hpp"
#include "tactile_qstring.hpp"
#include "to_map_document.hpp"
#include "to_property.hpp"

namespace tactile::parse {

template <typename Engine>
class MapParser final
{
 public:
  using engine_type = Engine;
  using document_type = typename engine_type::document_type;
  using object_type = typename engine_type::object_type;

  static_assert(IsEngine<engine_type, document_type, object_type>,
                "The supplied type isn't a parser engine!");

  explicit MapParser(const QString& path)
  {
    const QFileInfo info{path};
    mData.path = info.absoluteFilePath();
    if (const auto file = OpenFile(info))
    {
      const auto root = mEngine.Root(*file);
      if (!ParseNextLayerId(root))
      {
        return;
      }

      if (!ParseTilesets(root, info))
      {
        return;
      }

      if (!ParseLayers(root))
      {
        return;
      }

      if (auto properties = ParseProperties(root))
      {
        mData.properties = std::move(*properties);
      }
      else
      {
        return;
      }
    }
  }

  /**
   * \brief Creates and returns a map document based on the parsed data.
   *
   * \pre There mustn't have been any errors parsing the file.
   *
   * \details You must claim ownership of the returned pointer, preferably by
   * setting the parent of the map document as soon as possible.
   *
   * \return an owning pointer to the associated map document, can be null.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto MakeDocument() -> core::MapDocument*
  {
    Q_ASSERT(mError == ParseError::none);
    return ToMapDocument(mData);
  }

  /**
   * \brief Indicates whether or not the parser was successful.
   *
   * \return `true` if there were no errors when parsing the file; `false`
   * otherwise.
   *
   * \since 0.1.0
   */
  explicit operator bool() const noexcept
  {
    return mError == ParseError::none;
  }

  /**
   * \brief Returns the code associated with any potential error when parsing
   * the file.
   *
   * \return the error code associated with the parser.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto GetError() const noexcept -> ParseError
  {
    return mError;
  }

 private:
  engine_type mEngine;
  ParseError mError{ParseError::none};
  MapData mData;

  [[nodiscard]] auto WithError(const ParseError error) noexcept -> bool
  {
    mError = error;
    return false;
  }

  template <typename T>
  [[nodiscard]] auto WithError(const ParseError error) noexcept -> maybe<T>
  {
    mError = error;
    return std::nullopt;
  }

  [[nodiscard]] auto OpenFile(const QFileInfo& path) -> maybe<document_type>
  {
    if (!path.exists())
    {
      return WithError<document_type>(ParseError::map_file_not_found);
    }

    if (auto contents = mEngine.FromFile(path))
    {
      return contents;
    }
    else
    {
      return WithError<document_type>(ParseError::could_not_parse_file);
    }
  }

  [[nodiscard]] auto ParseNextLayerId(const object_type& root) -> bool
  {
    if (const auto id = root.Integer(ElementId::NextLayerId))
    {
      mData.nextLayerId = layer_id{*id};
      return true;
    }
    else
    {
      return WithError(ParseError::map_missing_next_layer_id);
    }
  }

  [[nodiscard]] auto ParseTilesetFirstGid(const object_type& object)
      -> maybe<tile_id>
  {
    if (const auto gid = object.Integer(ElementId::FirstGid))
    {
      return tile_id{*gid};
    }
    else
    {
      return WithError<tile_id>(ParseError::tileset_missing_first_gid);
    }
  }

  [[nodiscard]] auto ParseTilesetCommon(const object_type& object,
                                        const QFileInfo& path,
                                        const tile_id firstGid) -> bool
  {
    TilesetData tileset;
    tileset.firstId = firstGid;

    if (const auto tw = object.Integer(ElementId::TileWidth))
    {
      tileset.tileWidth = tile_width{*tw};
    }
    else
    {
      return WithError(ParseError::tileset_missing_tile_width);
    }

    if (const auto th = object.Integer(ElementId::TileHeight))
    {
      tileset.tileHeight = tile_height{*th};
    }
    else
    {
      return WithError(ParseError::tileset_missing_tile_height);
    }

    const auto relativePath = mEngine.TilesetImageRelativePath(object);
    if (!relativePath)
    {
      return WithError(ParseError::tileset_missing_image_path);
    }

    const auto absolutePath = path.dir().absoluteFilePath(*relativePath);
    if (QFileInfo{absolutePath}.exists())
    {
      tileset.absolutePath = absolutePath;
    }
    else
    {
      return WithError(ParseError::external_tileset_does_not_exist);
    }

    if (const auto name = object.String(ElementId::Name))
    {
      tileset.name = *name;
    }
    else
    {
      return WithError(ParseError::tileset_missing_name);
    }

    mData.tilesets.emplace_back(std::move(tileset));
    return true;
  }

  [[nodiscard]] auto ParseExternalTileset(const object_type& object,
                                          const QFileInfo& path,
                                          const tile_id firstGid) -> bool
  {
    const auto source = object.String(ElementId::Source);
    const auto external =
        mEngine.FromFile(QFileInfo{path.dir().absoluteFilePath(*source)});
    if (external)
    {
      return ParseTilesetCommon(mEngine.Root(*external), path, firstGid);
    }
    else
    {
      return WithError(ParseError::could_not_read_external_tileset);
    }
  }

  [[nodiscard]] auto ParseTileset(const object_type& object,
                                  const QFileInfo& path) -> bool
  {
    if (const auto gid = ParseTilesetFirstGid(object))
    {
      const auto hasSource = object.Contains(ElementId::Source);
      return hasSource ? ParseExternalTileset(object, path, *gid)
                       : ParseTilesetCommon(object, path, *gid);
    }
    else
    {
      return false;
    }
  }

  [[nodiscard]] auto ParseTilesets(const object_type& root,
                                   const QFileInfo& path) -> bool
  {
    if (!mEngine.ContainsTilesets(root))
    {
      return WithError(ParseError::map_missing_tilesets);
    }

    return std::ranges::all_of(
        mEngine.Tilesets(root),
        [&](const object_type& ts) { return ParseTileset(ts, path); });
  }

  [[nodiscard]] auto ParseTileLayer(LayerData& layer, const object_type& object)
      -> bool
  {
    TileLayerData tileLayer;

    if (const auto rows = object.Integer(ElementId::Height))
    {
      tileLayer.nRows = row_t{*rows};
    }
    else
    {
      return WithError(ParseError::layer_missing_height);
    }

    if (const auto cols = object.Integer(ElementId::Width))
    {
      tileLayer.nCols = col_t{*cols};
    }
    else
    {
      return WithError(ParseError::layer_missing_width);
    }

    tileLayer.tiles =
        mEngine.Tiles(object, tileLayer.nRows, tileLayer.nCols, mError);
    if (mError != ParseError::none)
    {
      return false;
    }

    layer.data = std::move(tileLayer);
    return true;
  }

  [[nodiscard]] auto ParseObjectLayer(LayerData& layer, const object_type& root)
      -> bool
  {
    ObjectLayerData objectLayer;

    for (const auto& elem : mEngine.Objects(root))
    {
      auto& object = objectLayer.objects.emplace_back();

      if (const auto id = elem.Integer(ElementId::Id))
      {
        object.id = object_id{*id};
      }
      else
      {
        return WithError(ParseError::object_missing_id);
      }

      object.x = elem.Floating(ElementId::X, 0);
      object.y = elem.Floating(ElementId::Y, 0);
      object.width = elem.Floating(ElementId::Width, 0);
      object.height = elem.Floating(ElementId::Height, 0);
      object.name = elem.String(ElementId::Name, QString{});
      object.customType = elem.String(ElementId::Type, QString{});
      object.visible = elem.Boolean(ElementId::Visible).value_or(true);
      object.isPoint = mEngine.IsPoint(elem);

      if (auto properties = ParseProperties(elem))
      {
        object.properties = std::move(*properties);
      }
      else
      {
        return false;
      }
    }

    layer.data = std::move(objectLayer);
    return true;
  }

  [[nodiscard]] auto ParseLayer(const object_type& object) -> bool
  {
    if (!mEngine.ValidateLayerType(object))
    {
      return WithError(ParseError::layer_missing_type);
    }

    LayerData layer;

    if (const auto id = object.Integer(ElementId::Id))
    {
      layer.id = layer_id{*id};
    }
    else
    {
      return WithError(ParseError::layer_missing_id);
    }

    layer.name = object.String(ElementId::Name, TACTILE_QSTRING(u"Layer"));
    layer.opacity = object.Floating(ElementId::Opacity, 1.0);
    layer.visible = object.Boolean(ElementId::Visible).value_or(true);

    if (mEngine.IsTileLayer(object))
    {
      layer.type = core::LayerType::tile_layer;

      if (!ParseTileLayer(layer, object))
      {
        return false;
      }
    }
    else if (mEngine.IsObjectLayer(object))
    {
      layer.type = core::LayerType::object_layer;

      if (!ParseObjectLayer(layer, object))
      {
        return false;
      }
    }
    else
    {
      return WithError(ParseError::unknown_layer_type);
    }

    if (auto properties = ParseProperties(object))
    {
      layer.properties = std::move(*properties);
    }
    else
    {
      return false;
    }

    mData.layers.emplace_back(std::move(layer));
    return true;
  }

  [[nodiscard]] auto ParseLayers(const object_type& root) -> bool
  {
    if (!mEngine.ContainsLayers(root))
    {
      return WithError(ParseError::map_missing_layers);
    }

    return std::ranges::all_of(
        mEngine.Layers(root),
        [this](const object_type& layer) { return ParseLayer(layer); });
  }

  [[nodiscard]] auto ParseProperty(const object_type& prop)
      -> maybe<PropertyData>
  {
    PropertyData data;
    data.name = prop.String(ElementId::Name).value();

    const auto type = mEngine.PropertyType(prop);
    if (auto property = ToProperty(prop, type, mError))
    {
      data.property = std::move(*property);
      return data;
    }
    else
    {
      return std::nullopt;
    }
  }

  [[nodiscard]] auto ParseProperties(const object_type& obj)
      -> maybe<std::vector<PropertyData>>
  {
    const auto props = mEngine.Properties(obj);

    std::vector<PropertyData> result;
    result.reserve(static_cast<usize>(props.size()));

    for (const auto& elem : props)
    {
      if (auto property = ParseProperty(elem))
      {
        result.emplace_back(*property);
      }
      else
      {
        return std::nullopt;
      }
    }

    return result;
  }
};

}  // namespace tactile::parse
