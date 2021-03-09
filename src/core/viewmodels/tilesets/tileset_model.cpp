#include "tileset_model.hpp"

#include "map_document.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

tileset_model::tileset_model(not_null<core::map_document*> document,
                             QObject* parent)
    : QStandardItemModel{parent}
    , m_document{document}
{
  if (!m_document) {
    throw tactile_error{"Cannot create tileset model from null document!"};
  }
}

void tileset_model::add(const QImage& image,
                        const QFileInfo& path,
                        const QString& name,
                        const tile_width tileWidth,
                        const tile_height tileHeight)
{
  m_document->add_tileset(image, path, name, tileWidth, tileHeight);
}

void tileset_model::remove(const QModelIndex& index)
{}

void tileset_model::select(const QModelIndex& index)
{}

void tileset_model::rename(const QModelIndex& index)
{}

void tileset_model::set_selection(const core::tileset_selection& selection)
{}

}  // namespace tactile::vm
