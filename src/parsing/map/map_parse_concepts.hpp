#pragma once

#include <QFileInfo>  // QFileInfo
#include <QString>    // QString
#include <concepts>   // same_as
#include <vector>     // vector

#include "map_parse_data.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "tile_layer.hpp"

namespace tactile {

// clang-format off

template <typename T>
concept IsParserObject = requires(T t, ElementId id, const QString& str)
{
  { t.Contains(id)      } -> std::same_as<bool>;
  { t.Integer(id)       } -> std::same_as<Maybe<int>>;
  { t.Integer(id, 1)    } -> std::same_as<Maybe<int>>;
  { t.Floating(id)      } -> std::same_as<Maybe<double>>;
  { t.Floating(id, 1.0) } -> std::same_as<double>;
  { t.String(id)        } -> std::same_as<Maybe<QString>>;
  { t.String(id, str)   } -> std::same_as<QString>;
  { t.Boolean(id)       } -> std::same_as<Maybe<bool>>;
};

template <typename Engine, typename Document, typename Object>
concept IsParserEngine = IsParserObject<Object> &&
                         requires(Engine e,
                                  const Document& document,
                                  const Object& object,
                                  const QFileInfo& path,
                                  ParseError& error,
                                  row_t nRows,
                                  col_t nCols)
{
  { e.FromFile(path) } -> std::same_as<Maybe<Document>>;
  { e.Root(document) } -> std::same_as<Object>;

  { e.Layers(object)                     } -> std::same_as<std::vector<Object>>;
  { e.Tilesets(object)                   } -> std::same_as<std::vector<Object>>;
  { e.Properties(object)                 } -> std::same_as<std::vector<Object>>;
  { e.Objects(object)                    } -> std::same_as<std::vector<Object>>;
  { e.Tiles(object, nRows, nCols, error) } -> std::same_as<core::tile_matrix>;

  { e.ContainsLayers(object)           } -> std::same_as<bool>;
  { e.ContainsTilesets(object)         } -> std::same_as<bool>;
  { e.ValidateLayerType(object)        } -> std::same_as<bool>;
  { e.TilesetImageRelativePath(object) } -> std::same_as<Maybe<QString>>;

  { e.IsObjectLayer(object) } -> std::same_as<bool>;
  { e.IsTileLayer(object)   } -> std::same_as<bool>;
  { e.IsPoint(object)       } -> std::same_as<bool>;
};

// clang-format on

}  // namespace tactile
